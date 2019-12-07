#ifndef HPP_B418A4F8FC204B14BE61E3002EFB4807

#define HPP_B418A4F8FC204B14BE61E3002EFB4807

#include <string>

namespace pdc {

struct config {
    int port = 1235;
};

config read_config (const std::string& filename);

} // namespace pdc

#endif // HPP_B418A4F8FC204B14BE61E3002EFB4807
