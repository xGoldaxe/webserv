#include "mime_types.hpp"

// trim from start
inline static std::string ltrim(std::string s)
{
	s.erase(s.begin(), std::find_if(s.begin(), s.end(),
									std::not1(std::ptr_fun<int, int>(std::isspace))));
	return s;
}

// trim from end
inline static std::string rtrim(std::string s)
{
	s.erase(std::find_if(s.rbegin(), s.rend(),
						 std::not1(std::ptr_fun<int, int>(std::isspace)))
				.base(),
			s.end());
	return s;
}

// trim from both ends
// inline static std::string trim(std::string s)
// {
// 	return ltrim(rtrim(s));
// }


MimeType::MimeType(const std::string name, const std::string extension, const std::string types) : _name(name), _extension(extension), _types(types)
{}

MimeType::MimeType(const MimeType &cpy) : _name(cpy._name), _extension(cpy._extension), _types(cpy._types)
{}

std::ostream &operator<<(std::ostream &os, const MimeType &dt)
{
    return (os << (dt._types + "/" + dt._name + " " + dt._extension));
}

MimeTypes::MimeTypes() : _map_extensions()
{}

void MimeTypes::setDefault()
{
    this->addMimeType("text", "html", "html");
    this->addMimeType("text", "html", "htm");
    this->addMimeType("text", "html", "shtml");
    this->addMimeType("text", "css", "css");
    this->addMimeType("text", "xml", "xml");
    this->addMimeType("image", "gif", "gif");
    this->addMimeType("image", "jpeg", "jpeg");
    this->addMimeType("image", "jpeg", "jpg");
    this->addMimeType("application", "javascript", "js");
    this->addMimeType("application", "atom+xml", "atom");
    this->addMimeType("application", "rss+xml", "rss");
    this->addMimeType("text", "mathml", "mml");
    this->addMimeType("text", "plain", "txt");
    this->addMimeType("text", "vnd.sun.j2me.app-descriptor", "jad");
    this->addMimeType("text", "vnd.wap.wml", "wml");
    this->addMimeType("text", "x-component", "htc");
    this->addMimeType("image", "avif", "avif");
    this->addMimeType("image", "png", "png");
    this->addMimeType("image", "svg+xml", "svg");
    this->addMimeType("image", "svg+xml", "svgz");
    this->addMimeType("image", "tiff", "tiff");
    this->addMimeType("image", "tiff", "tif");
    this->addMimeType("image", "vnd.wap.wbmp", "wbmp");
    this->addMimeType("image", "webp", "webp");
    this->addMimeType("image", "x-icon", "ico");
    this->addMimeType("image", "x-jng", "jng");
    this->addMimeType("image", "x-ms-bmp", "bmp");
    this->addMimeType("font", "woff", "woff");
    this->addMimeType("font", "woff2", "woff2");
    this->addMimeType("application", "java-archive", "jar");
    this->addMimeType("application", "java-archive", "ear");
    this->addMimeType("application", "java-archive", "war");
    this->addMimeType("application", "json", "json");
    this->addMimeType("application", "mac-binhex40", "hqx");
    this->addMimeType("application", "msword", "doc");
    this->addMimeType("application", "pdf", "pdf");
    this->addMimeType("application", "postscript", "ps");
    this->addMimeType("application", "postscript", "eps");
    this->addMimeType("application", "postscript", "ai");
    this->addMimeType("application", "rtf", "rtf");
    this->addMimeType("application", "vnd.apple.mpegurl", "m3u8");
    this->addMimeType("application", "vnd.google-earth.kml+xml", "kml");
    this->addMimeType("application", "vnd.google-earth.kmz", "kmz");
    this->addMimeType("application", "vnd.ms-excel", "xls");
    this->addMimeType("application", "vnd.ms-fontobject", "eot");
    this->addMimeType("application", "vnd.ms-powerpoint", "ppt");
    this->addMimeType("application", "vnd.oasis.opendocument.graphics", "odg");
    this->addMimeType("application", "vnd.oasis.opendocument.presentation", "odp");
    this->addMimeType("application", "vnd.oasis.opendocument.spreadsheet", "ods");
    this->addMimeType("application", "vnd.oasis.opendocument.text", "odt");
    this->addMimeType("application", "vnd.openxmlformats-officedocument.presentationml.presentation", "pptx");
    this->addMimeType("application", "vnd.openxmlformats-officedocument.spreadsheetml.sheet", "xlsx");
    this->addMimeType("application", "vnd.openxmlformats-officedocument.wordprocessingml.document", "docx");
    this->addMimeType("application", "vnd.wap.wmlc", "wmlc");
    this->addMimeType("application", "wasm", "wasm");
    this->addMimeType("application", "x-7z-compressed", "7z");
    this->addMimeType("application", "x-cocoa", "cco");
    this->addMimeType("application", "x-java-archive-diff", "jardiff");
    this->addMimeType("application", "x-java-jnlp-file", "jnlp");
    this->addMimeType("application", "x-makeself", "run");
    this->addMimeType("application", "x-perl", "pm");
    this->addMimeType("application", "x-perl", "pl");
    this->addMimeType("application", "x-pilot", "prc");
    this->addMimeType("application", "x-pilot", "pdb");
    this->addMimeType("application", "x-rar-compressed", "rar");
    this->addMimeType("application", "x-redhat-package-manager", "rpm");
    this->addMimeType("application", "x-sea", "sea");
    this->addMimeType("application", "x-shockwave-flash", "swf");
    this->addMimeType("application", "x-stuffit", "sit");
    this->addMimeType("application", "x-tcl", "tcl");
    this->addMimeType("application", "x-tcl", "tk");
    this->addMimeType("application", "x-x509-ca-cert", "der");
    this->addMimeType("application", "x-x509-ca-cert", "pem");
    this->addMimeType("application", "x-x509-ca-cert", "crt");
    this->addMimeType("application", "x-xpinstall", "xpi");
    this->addMimeType("application", "xhtml+xml", "xhtml");
    this->addMimeType("application", "xspf+xml", "xspf");
    this->addMimeType("application", "zip", "zip");
    this->addMimeType("application", "octet-stream", "bin");
    this->addMimeType("application", "octet-stream", "exe");
    this->addMimeType("application", "octet-stream", "dll");
    this->addMimeType("application", "octet-stream", "deb");
    this->addMimeType("application", "octet-stream", "dmg");
    this->addMimeType("application", "octet-stream", "iso");
    this->addMimeType("application", "octet-stream", "img");
    this->addMimeType("application", "octet-stream", "msi");
    this->addMimeType("application", "octet-stream", "msp");
    this->addMimeType("application", "octet-stream", "msm");
    this->addMimeType("audio", "midi", "mid");
    this->addMimeType("audio", "midi", "midi");
    this->addMimeType("audio", "midi", "kar");
    this->addMimeType("audio", "mpeg", "mp3");
    this->addMimeType("audio", "ogg", "ogg");
    this->addMimeType("audio", "x-m4a", "m4a");
    this->addMimeType("audio", "x-realaudio", "ra");
    this->addMimeType("video", "3gpp", "3gp");
    this->addMimeType("video", "3gpp", "3gpp");
    this->addMimeType("video", "mp2t", "ts");
    this->addMimeType("video", "mp4", "mp4");
    this->addMimeType("video", "mpeg", "mpeg");
    this->addMimeType("video", "mpeg", "mpg");
    this->addMimeType("video", "quicktime", "mov");
    this->addMimeType("video", "webm", "webm");
    this->addMimeType("video", "x-flv", "flv");
    this->addMimeType("video", "x-m4v", "m4v");
    this->addMimeType("video", "x-mng", "mng");
    this->addMimeType("video", "x-ms-asf", "asx");
    this->addMimeType("video", "x-ms-asf", "asf");
    this->addMimeType("video", "x-ms-wmv", "wmv");
    this->addMimeType("video", "x-msvideo", "avi");
}

MimeTypes::MimeTypes(const MimeTypes &cpy) : _map_extensions(cpy._map_extensions)
{}

MimeTypes &MimeTypes::operator=(const MimeTypes &cpy)
{
    if (this == &cpy)
        return *this;

    this->_map_extensions = cpy._map_extensions;
    return *this;
}

std::ostream &operator<<(std::ostream &os, const MimeTypes &dt)
{
    for (MimeTypes::_map_extensions_type::const_iterator it = dt._map_extensions.begin(); it != dt._map_extensions.end(); it++)
    {
        os << it->second;
        os << std::string("\n");
    }
    return os;
}

MimeTypes::~MimeTypes()
{}

static char asciitolower(char in) {
    return std::tolower(in);
}

/**
 * Determine le contenu du champ Content-Type:
 * @example Content-Type = (media-type)[OWS](;[OWS][params])
 * @param req the field line (without newline and not parsed)
 * @throw ExceptionContentType if the request is not well formatted
 * @return void
 */
void MimeTypes::parseHTTP(std::string req)
{
    if (req.find("Content-Type = ") > 0)
        throw ExceptionContentType();

    std::string content_type = req.substr(std::string("Content-Type = ").length(), req.length());
    std::string media_type = rtrim(content_type.substr(0, content_type.find(";")));

    #ifdef DEBUG
        std::cout << "Content-Type = " << content_type << std::endl;
        std::cout << "media type: |" << media_type << "|" << std::endl;
    #endif

    if (content_type.find(";") != std::string::npos) {
        // Remove les whitespace for options
        std::string media_options = ltrim(content_type.substr(content_type.find(";") + 1, content_type.length()));
        // Lower case the options
        std::transform(media_options.begin(), media_options.end(), media_options.begin(), asciitolower);

        #ifdef DEBUG
            std::cout << "args: |" << media_options << "|" << std::endl;
        #endif
    }
}

std::string MimeType::getContentType(void)
{
    return this->_types + "/" + this->_name;
}

/**
 * Get a mime type object for a given extension
 * @param extension a string without space and "."
 * @return MimeType the matching mime extension
 * @throw ExceptionUnknownMimeType if the extension doesn't exists in the server mime types
 */
MimeType MimeTypes::getMimeForExtension(std::string extension)
{
    if (this->_map_extensions.find(extension) == this->_map_extensions.end())
        throw new MimeType::ExceptionUnknownMimeType();

    return MimeType(this->_map_extensions.at(extension));
}

void MimeTypes::addMimeType(const std::string type, const std::string name, const std::string extension)
{
    this->_map_extensions.insert(std::make_pair(extension, MimeType(name, extension, type)));
}
