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
    this->addMimeType("html", "text", "html");
    this->addMimeType("html", "text", "htm");
    this->addMimeType("html", "text", "shtml");
    this->addMimeType("css", "text", "css");
    this->addMimeType("text", "xml", (const char *[]){"xml"});
    this->addMimeType("image", "gif", (const char *[]){"gif"});
    this->addMimeType("image", "jpeg", (const char *[]){"jpeg", "jpg"});
    this->addMimeType("application", "javascript", (const char *[]){"js"});
    this->addMimeType("application", "atom+xml", (const char *[]){"atom"});
    this->addMimeType("application", "rss+xml", (const char *[]){"rss"});
    this->addMimeType("text", "mathml", (const char *[]){"mml"});
    this->addMimeType("text", "plain", (const char *[]){"txt"});
    this->addMimeType("text", "vnd.sun.j2me.app-descriptor", (const char *[]){"jad"});
    this->addMimeType("text", "vnd.wap.wml", (const char *[]){"wml"});
    this->addMimeType("text", "x-component", (const char *[]){"htc"});
    this->addMimeType("image", "avif", (const char *[]){"avif"});
    this->addMimeType("image", "png", (const char *[]){"png"});
    this->addMimeType("image", "svg+xml", (const char *[]){"svg", "svgz"});
    this->addMimeType("image", "tiff", (const char *[]){"tif", "tiff"});
    this->addMimeType("image", "vnd.wap.wbmp", (const char *[]){"wbmp"});
    this->addMimeType("image", "webp", (const char *[]){"webp"});
    this->addMimeType("image", "x-icon", (const char *[]){"ico"});
    this->addMimeType("image", "x-jng", (const char *[]){"jng"});
    this->addMimeType("image", "x-ms-bmp", (const char *[]){"bmp"});
    this->addMimeType("font", "woff", (const char *[]){"woff"});
    this->addMimeType("font", "woff2", (const char *[]){"woff2"});
    this->addMimeType("application", "java-archive", (const char *[]){"jar", "war", "ear"});
    this->addMimeType("application", "json", (const char *[]){"json"});
    this->addMimeType("application", "mac-binhex40", (const char *[]){"hqx"});
    this->addMimeType("application", "msword", (const char *[]){"doc"});
    this->addMimeType("application", "pdf", (const char *[]){"pdf"});
    this->addMimeType("application", "postscript", (const char *[]){"ps", "eps", "ai"});
    this->addMimeType("application", "rtf", (const char *[]){"rtf"});
    this->addMimeType("application", "vnd.apple.mpegurl", (const char *[]){"m3u8"});
    this->addMimeType("application", "vnd.google-earth.kml+xml", (const char *[]){"kml"});
    this->addMimeType("application", "vnd.google-earth.kmz", (const char *[]){"kmz"});
    this->addMimeType("application", "vnd.ms-excel", (const char *[]){"xls"});
    this->addMimeType("application", "vnd.ms-fontobject", (const char *[]){"eot"});
    this->addMimeType("application", "vnd.ms-powerpoint", (const char *[]){"ppt"});
    this->addMimeType("application", "vnd.oasis.opendocument.graphics", (const char *[]){"odg"});
    this->addMimeType("application", "vnd.oasis.opendocument.presentation", (const char *[]){"odp"});
    this->addMimeType("application", "vnd.oasis.opendocument.spreadsheet", (const char *[]){"ods"});
    this->addMimeType("application", "vnd.oasis.opendocument.text", (const char *[]){"odt"});
    this->addMimeType("application", "vnd.openxmlformats-officedocument.presentationml.presentation", (const char *[]){"pptx"});
    this->addMimeType("application", "vnd.openxmlformats-officedocument.spreadsheetml.sheet", (const char *[]){"xlsx"});
    this->addMimeType("application", "vnd.openxmlformats-officedocument.wordprocessingml.document", (const char *[]){"docx"});
    this->addMimeType("application", "vnd.wap.wmlc", (const char *[]){"wmlc"});
    this->addMimeType("application", "wasm", (const char *[]){"wasm"});
    this->addMimeType("application", "x-7z-compressed", (const char *[]){"7z"});
    this->addMimeType("application", "x-cocoa", (const char *[]){"cco"});
    this->addMimeType("application", "x-java-archive-diff", (const char *[]){"jardiff"});
    this->addMimeType("application", "x-java-jnlp-file", (const char *[]){"jnlp"});
    this->addMimeType("application", "x-makeself", (const char *[]){"run"});
    this->addMimeType("application", "x-perl", (const char *[]){"pl", "pm"});
    this->addMimeType("application", "x-pilot", (const char *[]){"prc", "pdb"});
    this->addMimeType("application", "x-rar-compressed", (const char *[]){"rar"});
    this->addMimeType("application", "x-redhat-package-manager", (const char *[]){"rpm"});
    this->addMimeType("application", "x-sea", (const char *[]){"sea"});
    this->addMimeType("application", "x-shockwave-flash", (const char *[]){"swf"});
    this->addMimeType("application", "x-stuffit", (const char *[]){"sit"});
    this->addMimeType("application", "x-tcl", (const char *[]){"tcl", "tk"});
    this->addMimeType("application", "x-x509-ca-cert", (const char *[]){"der", "pem", "crt"});
    this->addMimeType("application", "x-xpinstall", (const char *[]){"xpi"});
    this->addMimeType("application", "xhtml+xml", (const char *[]){"xhtml"});
    this->addMimeType("application", "xspf+xml", (const char *[]){"xspf"});
    this->addMimeType("application", "zip", (const char *[]){"zip"});
    this->addMimeType("application", "octet-stream", (const char *[]){"bin", "exe", "dll"});
    this->addMimeType("application", "octet-stream", (const char *[]){"deb"});
    this->addMimeType("application", "octet-stream", (const char *[]){"dmg"});
    this->addMimeType("application", "octet-stream", (const char *[]){"iso", "img"});
    this->addMimeType("application", "octet-stream", (const char *[]){"msi", "msp", "msm"});
    this->addMimeType("audio", "midi", (const char *[]){"mid", "midi", "kar"});
    this->addMimeType("audio", "mpeg", (const char *[]){"mp3"});
    this->addMimeType("audio", "ogg", (const char *[]){"ogg"});
    this->addMimeType("audio", "x-m4a", (const char *[]){"m4a"});
    this->addMimeType("audio", "x-realaudio", (const char *[]){"ra"});
    this->addMimeType("video", "3gpp", (const char *[]){"3gpp", "3gp"});
    this->addMimeType("video", "mp2t", (const char *[]){"ts"});
    this->addMimeType("video", "mp4", (const char *[]){"mp4"});
    this->addMimeType("video", "mpeg", (const char *[]){"mpeg", "mpg"});
    this->addMimeType("video", "quicktime", (const char *[]){"mov"});
    this->addMimeType("video", "webm", (const char *[]){"webm"});
    this->addMimeType("video", "x-flv", (const char *[]){"flv"});
    this->addMimeType("video", "x-m4v", (const char *[]){"m4v"});
    this->addMimeType("video", "x-mng", (const char *[]){"mng"});
    this->addMimeType("video", "x-ms-asf", (const char *[]){"asx", "asf"});
    this->addMimeType("video", "x-ms-wmv", (const char *[]){"wmv"});
    this->addMimeType("video", "x-msvideo", (const char *[]){"avi"});
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
    return this->_types + "/" + this->_name + "; charset=utf-8";
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
        throw new ExceptionUnknownMimeType();

    return MimeType(this->_map_extensions.at(extension));
}

void MimeTypes::addMimeType(const std::string type, const std::string name, const std::string extension)
{
    this->_map_extensions.insert(std::make_pair(extension, MimeType(name, extension, type)));
}

void MimeTypes::addMimeType(const std::string type, const std::string name, const char *extensions[])
{
    int i = 0;
    while (extensions[i])
    {
        this->addMimeType(type, name, extensions[i++]);
    }
}
