#include "AsconfMainWindow.hpp"

#include <QMessageBox>
#include <QFileDialog>
#include <QPointer>
#include <QMenuBar>
#include <QWidget>
#include <QMenu>

#include <exception>
#include <type_traits>
#include <filesystem>
#include <algorithm>
#include <optional>
#include <iterator>
#include <cassert>
#include <fstream>
#include <string>
#include <vector>
#include <ios>

#include "msgpack.hpp"

#include "CregHandler.hpp"
#include "CprocExec.hpp"
#include "CprocPipe.hpp"
#include "CsmCodec.hpp"

namespace {
    const CompScanMap::MappingList ReadScancodeMap() noexcept {
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

    const AppSacnConf::MappingModel::container_type CurrentScancodeMap() noexcept {
        return ReadScancodeMap();
    }

    template <class byte_type>
    const std::optional<std::vector<byte_type>> ReadBinary(const std::filesystem::path& path) noexcept {
        std::ifstream stream(path, std::ios_base::binary | std::ios_base::in);
        if (!stream) {
            return std::nullopt;
        }
        std::vector<byte_type> output;
        using stream_iter = std::istreambuf_iterator<byte_type>;
        std::copy(
            stream_iter(stream), stream_iter(),
            std::back_insert_iterator(output)
        );
        return output;
    };

    const std::expected<CompScanMap::MappingList, std::string>  GetMappings(const std::vector<char>& bin) noexcept {
        msgpack::object_handle result;

        try {
            msgpack::unpack(result, bin.data(), bin.size());
        } catch(const msgpack::parse_error& e){
            return std::unexpected(e.what());
        }

        try {
            const auto obj = result.get();
            return obj.as<CompScanMap::MappingList>();
        }
        catch(const msgpack::type_error& e){
            return std::unexpected("the data format is invalid");
        }

        catch(const std::exception& e) {
            return std::unexpected(
                QString("unsupported error \"%1\" occurred").arg(e.what()).toStdString()
            );
        }

        catch(...) {
            return std::unexpected("unknown error occurred");
        }
    }
}

namespace msgpack {
MSGPACK_API_VERSION_NAMESPACE(MSGPACK_DEFAULT_API_NS) {
namespace adaptor {
    template <>
    struct convert<CompScanMap::ScanMapping> {
        msgpack::object const& operator()(msgpack::object const& obj, CompScanMap::ScanMapping& value) const {
            if (obj.type != type::ARRAY) {
                throw type_error();
            }
            if (obj.via.array.size != 2) throw type_error();

            value = {
                .to = obj.via.array.ptr[0].as<CompScanMap::Scancode>(),
                .from = obj.via.array.ptr[1].as<CompScanMap::Scancode>()
            };

            return obj;
        }
    };

    template<>
    struct pack<CompScanMap::ScanMapping> {
        template<class stream_type>
        packer<stream_type>& operator()(msgpack::packer<stream_type>& out, const CompScanMap::ScanMapping& value) const {
            out.pack_array(2);
            out.pack_uint16(value.to);
            out.pack_uint16(value.from);
            return out;
        }
    };

    template <>
    struct object_with_zone<CompScanMap::ScanMapping> {
        void operator()(msgpack::object::with_zone& obj, const CompScanMap::ScanMapping& value) const {
            obj.type = type::ARRAY;
            obj.via.array.size = 2;
            obj.via.array.ptr = static_cast<msgpack::object*>(
                obj.zone.allocate_align(sizeof(msgpack::object) * obj.via.array.size)
            );
            obj.via.array.ptr[0] = msgpack::object(value.to, obj.zone);
            obj.via.array.ptr[1] = msgpack::object(value.from, obj.zone);
        }
    };
}
}
}

namespace AppSacnConf {
    MainWindow::MainWindow() noexcept:
    QMainWindow(),
    m_mappingWidget(new MappingTableWidget(CurrentScancodeMap(), this)){
        setFixedSize(300, 300);

        setCentralWidget(m_mappingWidget);
        m_mappingWidget->setFixedWidth(width());

        createMenu();
        setWindowTitle(tr("Scancode Map Viewer"));
    }

    void MainWindow::createMenu() noexcept {
        auto fileMenu = menuBar()->addMenu(tr("&File"));

        const auto addMenuItem = []<class class_type>(QMenu* menu, const QString& text, class_type* obj, void (class_type::*func)()){
            static_assert(std::is_base_of_v<QObject, class_type>);
            QAction* act = new QAction(text);
            menu->addAction(act);
            connect(act, &QAction::triggered, obj, func);
        };
    
        addMenuItem(fileMenu, tr("&Import Mapping"), this, &MainWindow::importMapping);
        addMenuItem(fileMenu, tr("&Export Mapping"), this, &MainWindow::exportMapping);
    
        auto applyMenu = menuBar()->addMenu(tr("&Apply"));
        addMenuItem(applyMenu, tr("&Apply mapping"), this, &MainWindow::applyMapping);
    }

    void MainWindow::applyMapping() noexcept {
        // バイナリ書き込みプログラムに渡すパイプを作る
        const auto PipeName = CmpProc::CreateRandomPipeName();
        const auto Pipe = CmpProc::CreatePipe(PipeName, 1000);
        if (!Pipe) {
            QMessageBox::critical(
                this, QString(u8"Apply Error"),
                QString(u8"ライターの起動に失敗しました．\nエラーコード: %1").arg(Pipe.error())
            );
            return;
        }

        // パイプを閉じる
        DisconnectNamedPipe(Pipe.value().get());
    };
    
    void MainWindow::importMapping() noexcept {
        const auto ImportedFilePath = QFileDialog::getOpenFileName(
            this, QString(u8"Import mapping"), QString(),
            QString(u8"Mapping files (*.map);; All files(*.*)"), nullptr
        );
        if (ImportedFilePath.isEmpty()) {
            return;
        }

        const auto ImportedMapping = ReadBinary<char>(ImportedFilePath.toStdString());
        if (!ImportedMapping.has_value()) {
            QMessageBox::critical(
                this, QString(u8"Import error"),
                QString(u8"Can`t open \"%1\".").arg(ImportedFilePath));
            return;
        }

        const auto result = GetMappings(ImportedMapping.value());
        if (!result.has_value()) {
            QMessageBox::critical(
                this, QString(u8"Import error"),
                QString(u8"Parse failed: \"%1\".").arg(result.error().c_str()));
            return;
        }
        m_mappingWidget->setMappings(result.value());
    };
    
    void MainWindow::exportMapping() noexcept {
        const auto ExportedFilePath = QFileDialog::getSaveFileName(
            this, QString(u8"Export mapping"), QString(),
            QString(u8"Mapping files (*.map);; All files(*.*)"), nullptr
        );

        if (ExportedFilePath.isEmpty()) {
            return;
        }

        std::ofstream exportFile(std::filesystem::path(ExportedFilePath.toStdString()));
        msgpack::pack(&exportFile, m_mappingWidget->mappings());
        return;
    };

    void MainWindow::writeMapping(const CmpProc::object_handle& pipe, const CompReg::win32str& pipeName) noexcept {
        const auto Process = CmpProc::ExecElevated(false, "");
        if (!Process) {
            if (Process.error() == ERROR_CANCELLED) {
                return;
            }
            QMessageBox::critical(
                this, QString(u8"Apply Error"),
                QString(u8"ライターの起動に失敗しました．\nエラーコード: %1").arg(Process.error())
            );
            return;
        }

        // プログラムの終了を待つ
        const auto ExitCode = CmpProc::WaitUntilExit(*Process);
        if (!ExitCode) {
            QMessageBox::critical(
                this,QString(u8"Apply Error"),
                QString(u8"ライターの返値の取得に失敗しました\nエラーコード: %1.").arg(ExitCode.error())
            );
            return;
        }

        // プログラムの成否を伝える
        if (*ExitCode == 0) {
            QMessageBox::information(
                this,QString(u8"Apply Error"),
                QString(u8"レジストリへの書き込みが完了しました．")
            );
            return;
        }
        QMessageBox::critical(
            this,QString(u8"Apply Mapping"),
            QString(u8"レジストリへの書き込みに失敗しました．\nエラーコード: %1").arg(*ExitCode)
        );
    }
}