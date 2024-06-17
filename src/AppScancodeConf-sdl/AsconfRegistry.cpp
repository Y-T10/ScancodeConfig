#include "AsconfRegistry.hpp"

#include "CregHandler.hpp"
#include "CprocExec.hpp"
#include "CprocPipe.hpp"
#include "CsmCodec.hpp"

namespace {
    const bool writeMapping(const CmpProc::object_handle& pipe, const CompReg::win32str& pipeName, const CompScanMap::MappingList& mapping) noexcept {
        const auto GetParentDirectory = [](const CompReg::win32str& path){
            return path.substr(0, path.find_last_of('\\')) + TEXT('\\');
        };
        const auto GetProgramPath = []() {
            const CompReg::win32str Param = GetCommandLine();
            if (Param.contains(TEXT(' '))) {
                return Param.substr(0, Param.find(TEXT(' ')));
            }
            return Param;
        };
        const CompReg::win32str BinaryDir = GetParentDirectory(GetProgramPath());
        const CompReg::win32str WriterName = TEXT("AppScanMapWriter.exe");
        const auto Process = CmpProc::ExecElevated(false, BinaryDir + WriterName, pipeName);
        if (!Process) {
            if (Process.error() == ERROR_CANCELLED) {
                return false;
            }
            //QMessageBox::critical(
            //    this, QString(u8"Apply Error"),
            //    QString(u8"ライターの起動に失敗しました．\nエラーコード: %1").arg(Process.error())
            //);
            return false;
        }

        // ライターがパイプに接続するのを待つ
        if(ConnectNamedPipe(pipe.get(), nullptr) == 0) {
            //QMessageBox::critical(
            //    this, QString(u8"Apply Error"),
            //    QString(u8"ライターとの接続に失敗しました．\nエラーコード: %1").arg(GetLastError())
            //);
            return false;
        }

        // マッピングデータをバイナリに変換する
        const auto ToByte = [](const CompScanMap::MappingList& map){
            const size_t BinSize = map.size() * sizeof(CompScanMap::MappingList::value_type);
            auto bin = std::vector<uint8_t>(BinSize, 0);
            memcpy(bin.data(), map.data(), BinSize);
            return bin;
        };
        // パイプにデータを書き込む
        if (const auto result = CmpProc::WritePipe(pipe, ToByte(mapping)); !result.has_value()) {
            //QMessageBox::critical(
            //    this,QString(u8"Apply Error"),
            //    QString(u8"ライターへのデータ渡しに失敗しました: %1.").arg(result.error())
            //);
            return false;
        }
        FlushFileBuffers(pipe.get());

        // プログラムの終了を待つ
        const auto ExitCode = CmpProc::WaitUntilExit(*Process);
        if (!ExitCode) {
            //QMessageBox::critical(
            //    this,QString(u8"Apply Error"),
            //    QString(u8"ライターの返値の取得に失敗しました\nエラーコード: %1.").arg(ExitCode.error())
            //);
            return false;
        }

        // プログラムの成否を伝える
        if (*ExitCode == 0) {
            //QMessageBox::information(
            //    this,QString(u8"Apply Error"),
            //    QString(u8"レジストリへの書き込みが完了しました．")
            //);
            return false;
        }
        //QMessageBox::critical(
        //    this,QString(u8"Apply Mapping"),
        //    QString(u8"レジストリへの書き込みに失敗しました．\nエラーコード: %1").arg(*ExitCode)
        //);
        return false;
    }
}

const CompScanMap::MappingList AppSacnConf::ReadScancodeMap() noexcept {
    const auto RegKey = CompReg::OpenRegKey(
        HKEY_LOCAL_MACHINE,
        TEXT("SYSTEM\\CurrentControlSet\\Control\\Keyboard Layout"),
        KEY_READ
    );
    if (!RegKey) {
        return {};
    }
    const auto RegValue = ReadKeyValueBin(RegKey, TEXT("Scancode Map"));
    if (!RegValue) {
        return {};
    }
    const auto MappingList = CompScanMap::DecodeScancodeMap(*RegValue);
    return MappingList.has_value() ? *MappingList : CompScanMap::MappingList{};
}

const bool AppSacnConf::WriteScancodeMap(const CompScanMap::MappingList& mapping) noexcept {
    // バイナリ書き込みプログラムに渡すパイプを作る
    // TODO: パイプ名をランダムにする
    const auto PipeName = TEXT("\\\\.\\pipe\\ScancodeWritePipe");
    const auto Pipe = CmpProc::CreatePipe(PipeName, 0);
    if (!Pipe) {
        //QMessageBox::critical(
        //    this, QString(u8"Apply Error"),
        //    QString(u8"ライターの起動に失敗しました．\nエラーコード: %1").arg(Pipe.error())
        //);
        return false;
    }

    // パイプを用いてレジストリに値を書き込む
    if (!writeMapping(*Pipe, PipeName, mapping)) {
        return false;
    }

    // パイプを閉じる
    DisconnectNamedPipe(Pipe.value().get());

    return true;
}