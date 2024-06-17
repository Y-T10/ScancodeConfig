#include "AsconfMappingIO.hpp"

#include <cassert>
#include <format>
#include <fstream>

#include "msgpack.hpp"

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

namespace {
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
                std::format("unsupported error \"{:s}\" occurred", e.what())
            );
        }

        catch(...) {
            return std::unexpected("unknown error occurred");
        }
    }
}

namespace AppSacnConf {
    const CompScanMap::MappingList ImportMapping(const std::filesystem::path& filepath) noexcept {
        const auto ImportedMapping = ReadBinary<char>(filepath);
        if (!ImportedMapping.has_value()) {
            return {};
        }

        const auto result = GetMappings(ImportedMapping.value());
        if (!result.has_value()) {
            return {};
        }
        return result.value();
    }

    void ExportMapping(const std::filesystem::path& filepath, const CompScanMap::MappingList& mapping) noexcept {
        assert(!filepath.empty());

        std::ofstream exportFile(filepath);
        msgpack::pack(&exportFile, mapping);
    }
}