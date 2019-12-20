#ifndef HPP_B418A4F8FC204B14BE61E3002EFB4807

#define HPP_B418A4F8FC204B14BE61E3002EFB4807

#include <string>

namespace pdc {

/**
 * Configuration. All fields should be initialised in default constructor.
 */
struct config {
    int port = 1235;
};

/**
 * Parses JSON config. Throws on errors.
 * @param filename filename of config
 * @return parsed config
 */
config read_config (const std::string& filename);

} // namespace pdc

#endif // HPP_B418A4F8FC204B14BE61E3002EFB4807
