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


MimeType::MimeType(const std::string name, const std::string extension, const std::string types, const bool cgi_needed) : _name(name), _extension(extension), _types(types), _cgi(cgi_needed)
{}

MimeType::MimeType(const MimeType &cpy) : _name(cpy._name), _extension(cpy._extension), _types(cpy._types), _cgi(cpy._cgi)
{}

std::ostream &operator<<(std::ostream &os, const MimeType &dt)
{
    return (os << (dt._types + "/" + dt._name + " " + dt._extension));
}

bool MimeType::needCGI(void)
{
    return this->_cgi;
}

MimeTypes::MimeTypes() : _map_extensions()
{}

void MimeTypes::setDefault()
{
    this->addMimeType("application", "vnd.lotus-1-2-3", "123");
    this->addMimeType("text", "vnd.in3d.3dml", "3dml");
    this->addMimeType("video", "3gpp2", "3g2");
    this->addMimeType("video", "3gpp", "3gp");
    this->addMimeType("application", "octet-stream", "a");
    this->addMimeType("application", "x-authorware-bin", "aab");
    this->addMimeType("audio", "x-aac", "aac");
    this->addMimeType("application", "x-authorware-map", "aam");
    this->addMimeType("application", "x-authorware-seg", "aas");
    this->addMimeType("application", "x-abiword", "abw");
    this->addMimeType("application", "vnd.americandynamics.acc", "acc");
    this->addMimeType("application", "x-ace-compressed", "ace");
    this->addMimeType("application", "vnd.acucobol", "acu");
    this->addMimeType("application", "vnd.acucorp", "acutc");
    this->addMimeType("audio", "adpcm", "adp");
    this->addMimeType("application", "vnd.audiograph", "aep");
    this->addMimeType("application", "x-font-type1", "afm");
    this->addMimeType("application", "vnd.ibm.modcap", "afp");
    this->addMimeType("application", "postscript", "ai");
    this->addMimeType("audio", "x-aiff", "aif");
    this->addMimeType("audio", "x-aiff", "aifc");
    this->addMimeType("audio", "x-aiff", "aiff");
    this->addMimeType("application", "vnd.adobe.air-application-installer-package+zip", "air");
    this->addMimeType("application", "vnd.amiga.ami", "ami");
    this->addMimeType("application", "vnd.android.package-archive", "apk");
    this->addMimeType("application", "x-ms-application", "application");
    this->addMimeType("application", "vnd.lotus-approach", "apr");
    this->addMimeType("application", "pgp-signature", "asc");
    this->addMimeType("video", "x-ms-asf", "asf");
    this->addMimeType("text", "x-asm", "asm");
    this->addMimeType("application", "vnd.accpac.simply.aso", "aso");
    this->addMimeType("video", "x-ms-asf", "asx");
    this->addMimeType("application", "vnd.acucorp", "atc");
    this->addMimeType("application", "atom+xml", "atom");
    this->addMimeType("application", "atomcat+xml", "atomcat");
    this->addMimeType("application", "atomsvc+xml", "atomsvc");
    this->addMimeType("application", "vnd.antix.game-component", "atx");
    this->addMimeType("audio", "basic", "au");
    this->addMimeType("video", "x-msvideo", "avi");
    this->addMimeType("application", "applixware", "aw");
    this->addMimeType("application", "vnd.airzip.filesecure.azf", "azf");
    this->addMimeType("application", "vnd.airzip.filesecure.azs", "azs");
    this->addMimeType("application", "vnd.amazon.ebook", "azw");
    this->addMimeType("application", "x-msdownload", "bat");
    this->addMimeType("application", "x-bcpio", "bcpio");
    this->addMimeType("application", "x-font-bdf", "bdf");
    this->addMimeType("application", "vnd.syncml.dm+wbxml", "bdm");
    this->addMimeType("application", "vnd.fujitsu.oasysprs", "bh2");
    this->addMimeType("application", "octet-stream", "bin");
    this->addMimeType("application", "vnd.bmi", "bmi");
    this->addMimeType("image", "bmp", "bmp");
    this->addMimeType("application", "vnd.framemaker", "book");
    this->addMimeType("application", "vnd.previewsystems.box", "box");
    this->addMimeType("application", "x-bzip2", "boz");
    this->addMimeType("application", "octet-stream", "bpk");
    this->addMimeType("image", "prs.btif", "btif");
    this->addMimeType("application", "x-bzip", "bz");
    this->addMimeType("application", "x-bzip2", "bz2");
    this->addMimeType("text", "x-c", "c");
    this->addMimeType("application", "vnd.clonk.c4group", "c4d");
    this->addMimeType("application", "vnd.clonk.c4group", "c4f");
    this->addMimeType("application", "vnd.clonk.c4group", "c4g");
    this->addMimeType("application", "vnd.clonk.c4group", "c4p");
    this->addMimeType("application", "vnd.clonk.c4group", "c4u");
    this->addMimeType("application", "vnd.ms-cab-compressed", "cab");
    this->addMimeType("application", "vnd.curl.car", "car");
    this->addMimeType("application", "vnd.ms-pki.seccat", "cat");
    this->addMimeType("text", "x-c", "cc");
    this->addMimeType("application", "x-director", "cct");
    this->addMimeType("application", "ccxml+xml", "ccxml");
    this->addMimeType("application", "vnd.contact.cmsg", "cdbcmsg");
    this->addMimeType("application", "x-netcdf", "cdf");
    this->addMimeType("application", "vnd.mediastation.cdkey", "cdkey");
    this->addMimeType("chemical", "x-cdx", "cdx");
    this->addMimeType("application", "vnd.chemdraw+xml", "cdxml");
    this->addMimeType("application", "vnd.cinderella", "cdy");
    this->addMimeType("application", "pkix-cert", "cer");
    this->addMimeType("image", "cgm", "cgm");
    this->addMimeType("application", "x-chat", "chat");
    this->addMimeType("application", "vnd.ms-htmlhelp", "chm");
    this->addMimeType("application", "vnd.kde.kchart", "chrt");
    this->addMimeType("chemical", "x-cif", "cif");
    this->addMimeType("application", "vnd.anser-web-certificate-issue-initiation", "cii");
    this->addMimeType("application", "vnd.ms-artgalry", "cil");
    this->addMimeType("application", "vnd.claymore", "cla");
    this->addMimeType("application", "java-vm", "class");
    this->addMimeType("application", "vnd.crick.clicker.keyboard", "clkk");
    this->addMimeType("application", "vnd.crick.clicker.palette", "clkp");
    this->addMimeType("application", "vnd.crick.clicker.template", "clkt");
    this->addMimeType("application", "vnd.crick.clicker.wordbank", "clkw");
    this->addMimeType("application", "vnd.crick.clicker", "clkx");
    this->addMimeType("application", "x-msclip", "clp");
    this->addMimeType("application", "vnd.cosmocaller", "cmc");
    this->addMimeType("chemical", "x-cmdf", "cmdf");
    this->addMimeType("chemical", "x-cml", "cml");
    this->addMimeType("application", "vnd.yellowriver-custom-menu", "cmp");
    this->addMimeType("image", "x-cmx", "cmx");
    this->addMimeType("application", "vnd.rim.cod", "cod");
    this->addMimeType("application", "x-msdownload", "com");
    this->addMimeType("text", "plain", "conf");
    this->addMimeType("application", "x-cpio", "cpio");
    this->addMimeType("text", "x-c", "cpp");
    this->addMimeType("application", "mac-compactpro", "cpt");
    this->addMimeType("application", "x-mscardfile", "crd");
    this->addMimeType("application", "pkix-crl", "crl");
    this->addMimeType("application", "x-x509-ca-cert", "crt");
    this->addMimeType("application", "x-csh", "csh");
    this->addMimeType("chemical", "x-csml", "csml");
    this->addMimeType("application", "vnd.commonspace", "csp");
    this->addMimeType("text", "css", "css");
    this->addMimeType("application", "x-director", "cst");
    this->addMimeType("text", "csv", "csv");
    this->addMimeType("application", "cu-seeme", "cu");
    this->addMimeType("text", "vnd.curl", "curl");
    this->addMimeType("application", "prs.cww", "cww");
    this->addMimeType("application", "x-director", "cxt");
    this->addMimeType("text", "x-c", "cxx");
    this->addMimeType("application", "vnd.mobius.daf", "daf");
    this->addMimeType("application", "vnd.fdsn.seed", "dataless");
    this->addMimeType("application", "davmount+xml", "davmount");
    this->addMimeType("application", "x-director", "dcr");
    this->addMimeType("text", "vnd.curl.dcurl", "dcurl");
    this->addMimeType("application", "vnd.oma.dd2+xml", "dd2");
    this->addMimeType("application", "vnd.fujixerox.ddd", "ddd");
    this->addMimeType("application", "x-debian-package", "deb");
    this->addMimeType("text", "plain", "def");
    this->addMimeType("application", "octet-stream", "deploy");
    this->addMimeType("application", "x-x509-ca-cert", "der");
    this->addMimeType("application", "vnd.dreamfactory", "dfac");
    this->addMimeType("text", "x-c", "dic");
    this->addMimeType("text", "plain", "diff");
    this->addMimeType("application", "x-director", "dir");
    this->addMimeType("application", "vnd.mobius.dis", "dis");
    this->addMimeType("application", "octet-stream", "dist");
    this->addMimeType("application", "octet-stream", "distz");
    this->addMimeType("image", "vnd.djvu", "djv");
    this->addMimeType("image", "vnd.djvu", "djvu");
    this->addMimeType("application", "x-msdownload", "dll");
    this->addMimeType("application", "octet-stream", "dmg");
    this->addMimeType("application", "octet-stream", "dms");
    this->addMimeType("application", "vnd.dna", "dna");
    this->addMimeType("application", "msword", "doc");
    this->addMimeType("application", "vnd.ms-word.document.macroenabled.12", "docm");
    this->addMimeType("application", "vnd.openxmlformats-officedocument.wordprocessingml.document", "docx");
    this->addMimeType("application", "msword", "dot");
    this->addMimeType("application", "vnd.ms-word.template.macroenabled.12", "dotm");
    this->addMimeType("application", "vnd.openxmlformats-officedocument.wordprocessingml.template", "dotx");
    this->addMimeType("application", "vnd.osgi.dp", "dp");
    this->addMimeType("application", "vnd.dpgraph", "dpg");
    this->addMimeType("text", "prs.lines.tag", "dsc");
    this->addMimeType("application", "x-dtbook+xml", "dtb");
    this->addMimeType("application", "xml-dtd", "dtd");
    this->addMimeType("audio", "vnd.dts", "dts");
    this->addMimeType("audio", "vnd.dts.hd", "dtshd");
    this->addMimeType("application", "octet-stream", "dump");
    this->addMimeType("application", "x-dvi", "dvi");
    this->addMimeType("model", "vnd.dwf", "dwf");
    this->addMimeType("image", "vnd.dwg", "dwg");
    this->addMimeType("image", "vnd.dxf", "dxf");
    this->addMimeType("application", "vnd.spotfire.dxp", "dxp");
    this->addMimeType("application", "x-director", "dxr");
    this->addMimeType("audio", "vnd.nuera.ecelp4800", "ecelp4800");
    this->addMimeType("audio", "vnd.nuera.ecelp7470", "ecelp7470");
    this->addMimeType("audio", "vnd.nuera.ecelp9600", "ecelp9600");
    this->addMimeType("application", "ecmascript", "ecma");
    this->addMimeType("application", "vnd.novadigm.edm", "edm");
    this->addMimeType("application", "vnd.novadigm.edx", "edx");
    this->addMimeType("application", "vnd.picsel", "efif");
    this->addMimeType("application", "vnd.pg.osasli", "ei6");
    this->addMimeType("application", "octet-stream", "elc");
    this->addMimeType("message", "rfc822", "eml");
    this->addMimeType("application", "emma+xml", "emma");
    this->addMimeType("audio", "vnd.digital-winds", "eol");
    this->addMimeType("application", "vnd.ms-fontobject", "eot");
    this->addMimeType("application", "postscript", "eps");
    this->addMimeType("application", "epub+zip", "epub");
    this->addMimeType("application", "vnd.eszigno3+xml", "es3");
    this->addMimeType("application", "vnd.epson.esf", "esf");
    this->addMimeType("application", "vnd.eszigno3+xml", "et3");
    this->addMimeType("text", "x-setext", "etx");
    this->addMimeType("application", "x-msdownload", "exe");
    this->addMimeType("application", "vnd.novadigm.ext", "ext");
    this->addMimeType("application", "andrew-inset", "ez");
    this->addMimeType("application", "vnd.ezpix-album", "ez2");
    this->addMimeType("application", "vnd.ezpix-package", "ez3");
    this->addMimeType("text", "x-fortran", "f");
    this->addMimeType("video", "x-f4v", "f4v");
    this->addMimeType("text", "x-fortran", "f77");
    this->addMimeType("text", "x-fortran", "f90");
    this->addMimeType("image", "vnd.fastbidsheet", "fbs");
    this->addMimeType("application", "vnd.fdf", "fdf");
    this->addMimeType("application", "vnd.denovo.fcselayout-link", "fe_launch");
    this->addMimeType("application", "vnd.fujitsu.oasysgp", "fg5");
    this->addMimeType("application", "x-director", "fgd");
    this->addMimeType("image", "x-freehand", "fh");
    this->addMimeType("image", "x-freehand", "fh4");
    this->addMimeType("image", "x-freehand", "fh5");
    this->addMimeType("image", "x-freehand", "fh7");
    this->addMimeType("image", "x-freehand", "fhc");
    this->addMimeType("application", "x-xfig", "fig");
    this->addMimeType("video", "x-fli", "fli");
    this->addMimeType("application", "vnd.micrografx.flo", "flo");
    this->addMimeType("video", "x-flv", "flv");
    this->addMimeType("application", "vnd.kde.kivio", "flw");
    this->addMimeType("text", "vnd.fmi.flexstor", "flx");
    this->addMimeType("text", "vnd.fly", "fly");
    this->addMimeType("application", "vnd.framemaker", "fm");
    this->addMimeType("application", "vnd.frogans.fnc", "fnc");
    this->addMimeType("text", "x-fortran", "for");
    this->addMimeType("image", "vnd.fpx", "fpx");
    this->addMimeType("application", "vnd.framemaker", "frame");
    this->addMimeType("application", "vnd.fsc.weblaunch", "fsc");
    this->addMimeType("image", "vnd.fst", "fst");
    this->addMimeType("application", "vnd.fluxtime.clip", "ftc");
    this->addMimeType("application", "vnd.anser-web-funds-transfer-initiation", "fti");
    this->addMimeType("video", "vnd.fvt", "fvt");
    this->addMimeType("application", "vnd.fuzzysheet", "fzs");
    this->addMimeType("image", "g3fax", "g3");
    this->addMimeType("application", "vnd.groove-account", "gac");
    this->addMimeType("model", "vnd.gdl", "gdl");
    this->addMimeType("application", "vnd.dynageo", "geo");
    this->addMimeType("application", "vnd.geometry-explorer", "gex");
    this->addMimeType("application", "vnd.geogebra.file", "ggb");
    this->addMimeType("application", "vnd.geogebra.tool", "ggt");
    this->addMimeType("application", "vnd.groove-help", "ghf");
    this->addMimeType("image", "gif", "gif");
    this->addMimeType("application", "vnd.groove-identity-message", "gim");
    this->addMimeType("application", "vnd.gmx", "gmx");
    this->addMimeType("application", "x-gnumeric", "gnumeric");
    this->addMimeType("application", "vnd.flographit", "gph");
    this->addMimeType("application", "vnd.grafeq", "gqf");
    this->addMimeType("application", "vnd.grafeq", "gqs");
    this->addMimeType("application", "srgs", "gram");
    this->addMimeType("application", "vnd.geometry-explorer", "gre");
    this->addMimeType("application", "vnd.groove-injector", "grv");
    this->addMimeType("application", "srgs+xml", "grxml");
    this->addMimeType("application", "x-font-ghostscript", "gsf");
    this->addMimeType("application", "x-gtar", "gtar");
    this->addMimeType("application", "vnd.groove-tool-message", "gtm");
    this->addMimeType("model", "vnd.gtw", "gtw");
    this->addMimeType("text", "vnd.graphviz", "gv");
    this->addMimeType("application", "x-gzip", "gz");
    this->addMimeType("text", "x-c", "h");
    this->addMimeType("video", "h261", "h261");
    this->addMimeType("video", "h263", "h263");
    this->addMimeType("video", "h264", "h264");
    this->addMimeType("application", "vnd.hbci", "hbci");
    this->addMimeType("application", "x-hdf", "hdf");
    this->addMimeType("text", "x-c", "hh");
    this->addMimeType("application", "winhlp", "hlp");
    this->addMimeType("application", "vnd.hp-hpgl", "hpgl");
    this->addMimeType("application", "vnd.hp-hpid", "hpid");
    this->addMimeType("application", "vnd.hp-hps", "hps");
    this->addMimeType("application", "mac-binhex40", "hqx");
    this->addMimeType("application", "vnd.kenameaapp", "htke");
    this->addMimeType("text", "html", "htm");
    this->addMimeType("text", "html", "html");
    this->addMimeType("application", "vnd.yamaha.hv-dic", "hvd");
    this->addMimeType("application", "vnd.yamaha.hv-voice", "hvp");
    this->addMimeType("application", "vnd.yamaha.hv-script", "hvs");
    this->addMimeType("application", "vnd.iccprofile", "icc");
    this->addMimeType("x-conference", "x-cooltalk", "ice");
    this->addMimeType("application", "vnd.iccprofile", "icm");
    this->addMimeType("image", "x-icon", "ico");
    this->addMimeType("text", "calendar", "ics");
    this->addMimeType("image", "ief", "ief");
    this->addMimeType("text", "calendar", "ifb");
    this->addMimeType("application", "vnd.shana.informed.formdata", "ifm");
    this->addMimeType("model", "iges", "iges");
    this->addMimeType("application", "vnd.igloader", "igl");
    this->addMimeType("model", "iges", "igs");
    this->addMimeType("application", "vnd.micrografx.igx", "igx");
    this->addMimeType("application", "vnd.shana.informed.interchange", "iif");
    this->addMimeType("application", "vnd.accpac.simply.imp", "imp");
    this->addMimeType("application", "vnd.ms-ims", "ims");
    this->addMimeType("text", "plain", "in");
    this->addMimeType("application", "vnd.shana.informed.package", "ipk");
    this->addMimeType("application", "vnd.ibm.rights-management", "irm");
    this->addMimeType("application", "vnd.irepository.package+xml", "irp");
    this->addMimeType("application", "octet-stream", "iso");
    this->addMimeType("application", "vnd.shana.informed.formtemplate", "itp");
    this->addMimeType("application", "vnd.immervision-ivp", "ivp");
    this->addMimeType("application", "vnd.immervision-ivu", "ivu");
    this->addMimeType("text", "vnd.sun.j2me.app-descriptor", "jad");
    this->addMimeType("application", "vnd.jam", "jam");
    this->addMimeType("application", "java-archive", "jar");
    this->addMimeType("text", "x-java-source", "java");
    this->addMimeType("application", "vnd.jisp", "jisp");
    this->addMimeType("application", "vnd.hp-jlyt", "jlt");
    this->addMimeType("application", "x-java-jnlp-file", "jnlp");
    this->addMimeType("application", "vnd.joost.joda-archive", "joda");
    this->addMimeType("image", "jpeg", "jpe");
    this->addMimeType("image", "jpeg", "jpeg");
    this->addMimeType("image", "jpeg", "jpg");
    this->addMimeType("video", "jpm", "jpgm");
    this->addMimeType("video", "jpeg", "jpgv");
    this->addMimeType("video", "jpm", "jpm");
    this->addMimeType("application", "javascript", "js");
    this->addMimeType("application", "json", "json");
    this->addMimeType("audio", "midi", "kar");
    this->addMimeType("application", "vnd.kde.karbon", "karbon");
    this->addMimeType("application", "vnd.kde.kformula", "kfo");
    this->addMimeType("application", "vnd.kidspiration", "kia");
    this->addMimeType("application", "x-killustrator", "kil");
    this->addMimeType("application", "vnd.google-earth.kml+xml", "kml");
    this->addMimeType("application", "vnd.google-earth.kmz", "kmz");
    this->addMimeType("application", "vnd.kinar", "kne");
    this->addMimeType("application", "vnd.kinar", "knp");
    this->addMimeType("application", "vnd.kde.kontour", "kon");
    this->addMimeType("application", "vnd.kde.kpresenter", "kpr");
    this->addMimeType("application", "vnd.kde.kpresenter", "kpt");
    this->addMimeType("text", "plain", "ksh");
    this->addMimeType("application", "vnd.kde.kspread", "ksp");
    this->addMimeType("application", "vnd.kahootz", "ktr");
    this->addMimeType("application", "vnd.kahootz", "ktz");
    this->addMimeType("application", "vnd.kde.kword", "kwd");
    this->addMimeType("application", "vnd.kde.kword", "kwt");
    this->addMimeType("application", "x-latex", "latex");
    this->addMimeType("application", "vnd.llamagraphics.life-balance.desktop", "lbd");
    this->addMimeType("application", "vnd.llamagraphics.life-balance.exchange+xml", "lbe");
    this->addMimeType("application", "vnd.hhe.lesson-player", "les");
    this->addMimeType("application", "octet-stream", "lha");
    this->addMimeType("application", "vnd.route66.link66+xml", "link66");
    this->addMimeType("text", "plain", "list");
    this->addMimeType("application", "vnd.ibm.modcap", "list3820");
    this->addMimeType("application", "vnd.ibm.modcap", "listafp");
    this->addMimeType("text", "plain", "log");
    this->addMimeType("application", "lost+xml", "lostxml");
    this->addMimeType("application", "octet-stream", "lrf");
    this->addMimeType("application", "vnd.ms-lrm", "lrm");
    this->addMimeType("application", "vnd.frogans.ltf", "ltf");
    this->addMimeType("audio", "vnd.lucent.voice", "lvp");
    this->addMimeType("application", "vnd.lotus-wordpro", "lwp");
    this->addMimeType("application", "octet-stream", "lzh");
    this->addMimeType("application", "x-msmediaview", "m13");
    this->addMimeType("application", "x-msmediaview", "m14");
    this->addMimeType("video", "mpeg", "m1v");
    this->addMimeType("audio", "mpeg", "m2a");
    this->addMimeType("video", "mpeg", "m2v");
    this->addMimeType("audio", "mpeg", "m3a");
    this->addMimeType("audio", "x-mpegurl", "m3u");
    this->addMimeType("video", "vnd.mpegurl", "m4u");
    this->addMimeType("video", "x-m4v", "m4v");
    this->addMimeType("application", "mathematica", "ma");
    this->addMimeType("application", "vnd.ecowin.chart", "mag");
    this->addMimeType("application", "vnd.framemaker", "maker");
    this->addMimeType("text", "troff", "man");
    this->addMimeType("application", "mathml+xml", "mathml");
    this->addMimeType("application", "mathematica", "mb");
    this->addMimeType("application", "vnd.mobius.mbk", "mbk");
    this->addMimeType("application", "mbox", "mbox");
    this->addMimeType("application", "vnd.medcalcdata", "mc1");
    this->addMimeType("application", "vnd.mcd", "mcd");
    this->addMimeType("text", "vnd.curl.mcurl", "mcurl");
    this->addMimeType("application", "x-msaccess", "mdb");
    this->addMimeType("image", "vnd.ms-modi", "mdi");
    this->addMimeType("text", "troff", "me");
    this->addMimeType("model", "mesh", "mesh");
    this->addMimeType("application", "vnd.mfmp", "mfm");
    this->addMimeType("application", "vnd.proteus.magazine", "mgz");
    this->addMimeType("message", "rfc822", "mht");
    this->addMimeType("message", "rfc822", "mhtml");
    this->addMimeType("audio", "midi", "mid");
    this->addMimeType("audio", "midi", "midi");
    this->addMimeType("application", "vnd.mif", "mif");
    this->addMimeType("message", "rfc822", "mime");
    this->addMimeType("video", "mj2", "mj2");
    this->addMimeType("video", "mj2", "mjp2");
    this->addMimeType("application", "vnd.dolby.mlp", "mlp");
    this->addMimeType("application", "vnd.chipnuts.karaoke-mmd", "mmd");
    this->addMimeType("application", "vnd.smaf", "mmf");
    this->addMimeType("image", "vnd.fujixerox.edmics-mmr", "mmr");
    this->addMimeType("application", "x-msmoney", "mny");
    this->addMimeType("application", "x-mobipocket-ebook", "mobi");
    this->addMimeType("video", "quicktime", "mov");
    this->addMimeType("video", "x-sgi-movie", "movie");
    this->addMimeType("audio", "mpeg", "mp2");
    this->addMimeType("audio", "mpeg", "mp2a");
    this->addMimeType("audio", "mpeg", "mp3");
    this->addMimeType("video", "mp4", "mp4");
    this->addMimeType("audio", "mp4", "mp4a");
    this->addMimeType("application", "mp4", "mp4s");
    this->addMimeType("video", "mp4", "mp4v");
    this->addMimeType("video", "mpeg", "mpa");
    this->addMimeType("application", "vnd.mophun.certificate", "mpc");
    this->addMimeType("video", "mpeg", "mpe");
    this->addMimeType("video", "mpeg", "mpeg");
    this->addMimeType("video", "mpeg", "mpg");
    this->addMimeType("video", "mp4", "mpg4");
    this->addMimeType("audio", "mpeg", "mpga");
    this->addMimeType("application", "vnd.apple.installer+xml", "mpkg");
    this->addMimeType("application", "vnd.blueice.multipass", "mpm");
    this->addMimeType("application", "vnd.mophun.application", "mpn");
    this->addMimeType("application", "vnd.ms-project", "mpp");
    this->addMimeType("application", "vnd.ms-project", "mpt");
    this->addMimeType("application", "vnd.ibm.minipay", "mpy");
    this->addMimeType("application", "vnd.mobius.mqy", "mqy");
    this->addMimeType("application", "marc", "mrc");
    this->addMimeType("text", "troff", "ms");
    this->addMimeType("application", "mediaservercontrol+xml", "mscml");
    this->addMimeType("application", "vnd.fdsn.mseed", "mseed");
    this->addMimeType("application", "vnd.mseq", "mseq");
    this->addMimeType("application", "vnd.epson.msf", "msf");
    this->addMimeType("model", "mesh", "msh");
    this->addMimeType("application", "x-msdownload", "msi");
    this->addMimeType("application", "vnd.mobius.msl", "msl");
    this->addMimeType("application", "vnd.muvee.style", "msty");
    this->addMimeType("model", "vnd.mts", "mts");
    this->addMimeType("application", "vnd.musician", "mus");
    this->addMimeType("application", "vnd.recordare.musicxml+xml", "musicxml");
    this->addMimeType("application", "x-msmediaview", "mvb");
    this->addMimeType("application", "vnd.mfer", "mwf");
    this->addMimeType("application", "mxf", "mxf");
    this->addMimeType("application", "vnd.recordare.musicxml", "mxl");
    this->addMimeType("application", "xv+xml", "mxml");
    this->addMimeType("application", "vnd.triscape.mxs", "mxs");
    this->addMimeType("video", "vnd.mpegurl", "mxu");
    this->addMimeType("application", "vnd.nokia.n-gage.symbian.install", "n-gage");
    this->addMimeType("application", "mathematica", "nb");
    this->addMimeType("application", "x-netcdf", "nc");
    this->addMimeType("application", "x-dtbncx+xml", "ncx");
    this->addMimeType("application", "vnd.nokia.n-gage.data", "ngdat");
    this->addMimeType("application", "vnd.neurolanguage.nlu", "nlu");
    this->addMimeType("application", "vnd.enliven", "nml");
    this->addMimeType("application", "vnd.noblenet-directory", "nnd");
    this->addMimeType("application", "vnd.noblenet-sealer", "nns");
    this->addMimeType("application", "vnd.noblenet-web", "nnw");
    this->addMimeType("image", "vnd.net-fpx", "npx");
    this->addMimeType("application", "vnd.lotus-notes", "nsf");
    this->addMimeType("message", "rfc822", "nws");
    this->addMimeType("application", "octet-stream", "o");
    this->addMimeType("application", "vnd.fujitsu.oasys2", "oa2");
    this->addMimeType("application", "vnd.fujitsu.oasys3", "oa3");
    this->addMimeType("application", "vnd.fujitsu.oasys", "oas");
    this->addMimeType("application", "x-msbinder", "obd");
    this->addMimeType("application", "octet-stream", "obj");
    this->addMimeType("application", "oda", "oda");
    this->addMimeType("application", "vnd.oasis.opendocument.database", "odb");
    this->addMimeType("application", "vnd.oasis.opendocument.chart", "odc");
    this->addMimeType("application", "vnd.oasis.opendocument.formula", "odf");
    this->addMimeType("application", "vnd.oasis.opendocument.formula-template", "odft");
    this->addMimeType("application", "vnd.oasis.opendocument.graphics", "odg");
    this->addMimeType("application", "vnd.oasis.opendocument.image", "odi");
    this->addMimeType("application", "vnd.oasis.opendocument.presentation", "odp");
    this->addMimeType("application", "vnd.oasis.opendocument.spreadsheet", "ods");
    this->addMimeType("application", "vnd.oasis.opendocument.text", "odt");
    this->addMimeType("audio", "ogg", "oga");
    this->addMimeType("audio", "ogg", "ogg");
    this->addMimeType("video", "ogg", "ogv");
    this->addMimeType("application", "ogg", "ogx");
    this->addMimeType("application", "onenote", "onepkg");
    this->addMimeType("application", "onenote", "onetmp");
    this->addMimeType("application", "onenote", "onetoc");
    this->addMimeType("application", "onenote", "onetoc2");
    this->addMimeType("application", "oebps-package+xml", "opf");
    this->addMimeType("application", "vnd.palm", "oprc");
    this->addMimeType("application", "vnd.lotus-organizer", "org");
    this->addMimeType("application", "vnd.yamaha.openscoreformat", "osf");
    this->addMimeType("application", "vnd.yamaha.openscoreformat.osfpvg+xml", "osfpvg");
    this->addMimeType("application", "vnd.oasis.opendocument.chart-template", "otc");
    this->addMimeType("application", "x-font-otf", "otf");
    this->addMimeType("application", "vnd.oasis.opendocument.graphics-template", "otg");
    this->addMimeType("application", "vnd.oasis.opendocument.text-web", "oth");
    this->addMimeType("application", "vnd.oasis.opendocument.image-template", "oti");
    this->addMimeType("application", "vnd.oasis.opendocument.text-master", "otm");
    this->addMimeType("application", "vnd.oasis.opendocument.presentation-template", "otp");
    this->addMimeType("application", "vnd.oasis.opendocument.spreadsheet-template", "ots");
    this->addMimeType("application", "vnd.oasis.opendocument.text-template", "ott");
    this->addMimeType("application", "vnd.openofficeorg.extension", "oxt");
    this->addMimeType("text", "x-pascal", "p");
    this->addMimeType("application", "pkcs10", "p10");
    this->addMimeType("application", "x-pkcs12", "p12");
    this->addMimeType("application", "x-pkcs7-certificates", "p7b");
    this->addMimeType("application", "pkcs7-mime", "p7c");
    this->addMimeType("application", "pkcs7-mime", "p7m");
    this->addMimeType("application", "x-pkcs7-certreqresp", "p7r");
    this->addMimeType("application", "pkcs7-signature", "p7s");
    this->addMimeType("text", "x-pascal", "pas");
    this->addMimeType("application", "vnd.powerbuilder6", "pbd");
    this->addMimeType("image", "x-portable-bitmap", "pbm");
    this->addMimeType("application", "x-font-pcf", "pcf");
    this->addMimeType("application", "vnd.hp-pcl", "pcl");
    this->addMimeType("application", "vnd.hp-pclxl", "pclxl");
    this->addMimeType("image", "x-pict", "pct");
    this->addMimeType("application", "vnd.curl.pcurl", "pcurl");
    this->addMimeType("image", "x-pcx", "pcx");
    this->addMimeType("application", "vnd.palm", "pdb");
    this->addMimeType("application", "pdf", "pdf");
    this->addMimeType("application", "x-font-type1", "pfa");
    this->addMimeType("application", "x-font-type1", "pfb");
    this->addMimeType("application", "x-font-type1", "pfm");
    this->addMimeType("application", "font-tdpfr", "pfr");
    this->addMimeType("application", "x-pkcs12", "pfx");
    this->addMimeType("image", "x-portable-graymap", "pgm");
    this->addMimeType("application", "x-chess-pgn", "pgn");
    this->addMimeType("application", "pgp-encrypted", "pgp");
    this->addMimeType("image", "x-pict", "pic");
    this->addMimeType("application", "octet-stream", "pkg");
    this->addMimeType("application", "pkixcmp", "pki");
    this->addMimeType("application", "pkix-pkipath", "pkipath");
    this->addMimeType("text", "plain", "pl");
    this->addMimeType("application", "vnd.3gpp.pic-bw-large", "plb");
    this->addMimeType("application", "vnd.mobius.plc", "plc");
    this->addMimeType("application", "vnd.pocketlearn", "plf");
    this->addMimeType("application", "pls+xml", "pls");
    this->addMimeType("application", "vnd.ctc-posml", "pml");
    this->addMimeType("image", "png", "png");
    this->addMimeType("image", "x-portable-anymap", "pnm");
    this->addMimeType("application", "vnd.macports.portpkg", "portpkg");
    this->addMimeType("application", "vnd.ms-powerpoint", "pot");
    this->addMimeType("application", "vnd.ms-powerpoint.template.macroenabled.12", "potm");
    this->addMimeType("application", "vnd.openxmlformats-officedocument.presentationml.template", "potx");
    this->addMimeType("application", "vnd.ms-powerpoint", "ppa");
    this->addMimeType("application", "vnd.ms-powerpoint.addin.macroenabled.12", "ppam");
    this->addMimeType("application", "vnd.cups-ppd", "ppd");
    this->addMimeType("image", "x-portable-pixmap", "ppm");
    this->addMimeType("application", "vnd.ms-powerpoint", "pps");
    this->addMimeType("application", "vnd.ms-powerpoint.slideshow.macroenabled.12", "ppsm");
    this->addMimeType("application", "vnd.openxmlformats-officedocument.presentationml.slideshow", "ppsx");
    this->addMimeType("application", "vnd.ms-powerpoint", "ppt");
    this->addMimeType("application", "vnd.ms-powerpoint.presentation.macroenabled.12", "pptm");
    this->addMimeType("application", "vnd.openxmlformats-officedocument.presentationml.presentation", "pptx");
    this->addMimeType("application", "vnd.palm", "pqa");
    this->addMimeType("application", "x-mobipocket-ebook", "prc");
    this->addMimeType("application", "vnd.lotus-freelance", "pre");
    this->addMimeType("application", "pics-rules", "prf");
    this->addMimeType("application", "postscript", "ps");
    this->addMimeType("application", "vnd.3gpp.pic-bw-small", "psb");
    this->addMimeType("image", "vnd.adobe.photoshop", "psd");
    this->addMimeType("application", "x-font-linux-psf", "psf");
    this->addMimeType("application", "vnd.pvi.ptid1", "ptid");
    this->addMimeType("application", "x-mspublisher", "pub");
    this->addMimeType("application", "vnd.3gpp.pic-bw-var", "pvb");
    this->addMimeType("application", "vnd.3m.post-it-notes", "pwn");
    this->addMimeType("application", "vnd.ms-powerpoint", "pwz");
    this->addMimeType("text", "x-python", "py");
    this->addMimeType("audio", "vnd.ms-playready.media.pya", "pya");
    this->addMimeType("application", "x-python-code", "pyc");
    this->addMimeType("application", "x-python-code", "pyo");
    this->addMimeType("video", "vnd.ms-playready.media.pyv", "pyv");
    this->addMimeType("application", "vnd.epson.quickanime", "qam");
    this->addMimeType("application", "vnd.intu.qbo", "qbo");
    this->addMimeType("application", "vnd.intu.qfx", "qfx");
    this->addMimeType("application", "vnd.publishare-delta-tree", "qps");
    this->addMimeType("video", "quicktime", "qt");
    this->addMimeType("application", "vnd.quark.quarkxpress", "qwd");
    this->addMimeType("application", "vnd.quark.quarkxpress", "qwt");
    this->addMimeType("application", "vnd.quark.quarkxpress", "qxb");
    this->addMimeType("application", "vnd.quark.quarkxpress", "qxd");
    this->addMimeType("application", "vnd.quark.quarkxpress", "qxl");
    this->addMimeType("application", "vnd.quark.quarkxpress", "qxt");
    this->addMimeType("audio", "x-pn-realaudio", "ra");
    this->addMimeType("audio", "x-pn-realaudio", "ram");
    this->addMimeType("application", "x-rar-compressed", "rar");
    this->addMimeType("image", "x-cmu-raster", "ras");
    this->addMimeType("application", "vnd.ipunplugged.rcprofile", "rcprofile");
    this->addMimeType("application", "rdf+xml", "rdf");
    this->addMimeType("application", "vnd.data-vision.rdz", "rdz");
    this->addMimeType("application", "vnd.businessobjects", "rep");
    this->addMimeType("application", "x-dtbresource+xml", "res");
    this->addMimeType("image", "x-rgb", "rgb");
    this->addMimeType("application", "reginfo+xml", "rif");
    this->addMimeType("application", "resource-lists+xml", "rl");
    this->addMimeType("image", "vnd.fujixerox.edmics-rlc", "rlc");
    this->addMimeType("application", "resource-lists-diff+xml", "rld");
    this->addMimeType("application", "vnd.rn-realmedia", "rm");
    this->addMimeType("audio", "midi", "rmi");
    this->addMimeType("audio", "x-pn-realaudio-plugin", "rmp");
    this->addMimeType("application", "vnd.jcp.javame.midlet-rms", "rms");
    this->addMimeType("application", "relax-ng-compact-syntax", "rnc");
    this->addMimeType("text", "troff", "roff");
    this->addMimeType("application", "x-rpm", "rpm");
    this->addMimeType("application", "vnd.nokia.radio-presets", "rpss");
    this->addMimeType("application", "vnd.nokia.radio-preset", "rpst");
    this->addMimeType("application", "sparql-query", "rq");
    this->addMimeType("application", "rls-services+xml", "rs");
    this->addMimeType("application", "rsd+xml", "rsd");
    this->addMimeType("application", "rss+xml", "rss");
    this->addMimeType("application", "rtf", "rtf");
    this->addMimeType("text", "richtext", "rtx");
    this->addMimeType("text", "x-asm", "s");
    this->addMimeType("application", "vnd.yamaha.smaf-audio", "saf");
    this->addMimeType("application", "sbml+xml", "sbml");
    this->addMimeType("application", "vnd.ibm.secure-container", "sc");
    this->addMimeType("application", "x-msschedule", "scd");
    this->addMimeType("application", "vnd.lotus-screencam", "scm");
    this->addMimeType("application", "scvp-cv-request", "scq");
    this->addMimeType("application", "scvp-cv-response", "scs");
    this->addMimeType("text", "vnd.curl.scurl", "scurl");
    this->addMimeType("application", "vnd.stardivision.draw", "sda");
    this->addMimeType("application", "vnd.stardivision.calc", "sdc");
    this->addMimeType("application", "vnd.stardivision.impress", "sdd");
    this->addMimeType("application", "vnd.solent.sdkm+xml", "sdkd");
    this->addMimeType("application", "vnd.solent.sdkm+xml", "sdkm");
    this->addMimeType("application", "sdp", "sdp");
    this->addMimeType("application", "vnd.stardivision.writer", "sdw");
    this->addMimeType("application", "vnd.seemail", "see");
    this->addMimeType("application", "vnd.fdsn.seed", "seed");
    this->addMimeType("application", "vnd.sema", "sema");
    this->addMimeType("application", "vnd.semd", "semd");
    this->addMimeType("application", "vnd.semf", "semf");
    this->addMimeType("application", "java-serialized-object", "ser");
    this->addMimeType("application", "set-payment-initiation", "setpay");
    this->addMimeType("application", "set-registration-initiation", "setreg");
    this->addMimeType("application", "vnd.hydrostatix.sof-data", "sfd-hdstx");
    this->addMimeType("application", "vnd.spotfire.sfs", "sfs");
    this->addMimeType("application", "vnd.stardivision.writer-global", "sgl");
    this->addMimeType("text", "sgml", "sgm");
    this->addMimeType("text", "sgml", "sgml");
    this->addMimeType("application", "x-sh", "sh");
    this->addMimeType("application", "x-shar", "shar");
    this->addMimeType("application", "shf+xml", "shf");
    this->addMimeType("text", "vnd.wap.si", "si");
    this->addMimeType("application", "vnd.wap.sic", "sic");
    this->addMimeType("application", "pgp-signature", "sig");
    this->addMimeType("model", "mesh", "silo");
    this->addMimeType("application", "vnd.symbian.install", "sis");
    this->addMimeType("application", "vnd.symbian.install", "sisx");
    this->addMimeType("application", "x-stuffit", "sit");
    this->addMimeType("application", "x-stuffitx", "sitx");
    this->addMimeType("application", "vnd.koan", "skd");
    this->addMimeType("application", "vnd.koan", "skm");
    this->addMimeType("application", "vnd.koan", "skp");
    this->addMimeType("application", "vnd.koan", "skt");
    this->addMimeType("text", "vnd.wap.sl", "sl");
    this->addMimeType("application", "vnd.wap.slc", "slc");
    this->addMimeType("application", "vnd.ms-powerpoint.slide.macroenabled.12", "sldm");
    this->addMimeType("application", "vnd.openxmlformats-officedocument.presentationml.slide", "sldx");
    this->addMimeType("application", "vnd.epson.salt", "slt");
    this->addMimeType("application", "vnd.stardivision.math", "smf");
    this->addMimeType("application", "smil+xml", "smi");
    this->addMimeType("application", "smil+xml", "smil");
    this->addMimeType("audio", "basic", "snd");
    this->addMimeType("application", "x-font-snf", "snf");
    this->addMimeType("application", "octet-stream", "so");
    this->addMimeType("application", "x-pkcs7-certificates", "spc");
    this->addMimeType("application", "vnd.yamaha.smaf-phrase", "spf");
    this->addMimeType("application", "x-futuresplash", "spl");
    this->addMimeType("text", "vnd.in3d.spot", "spot");
    this->addMimeType("application", "scvp-vp-response", "spp");
    this->addMimeType("application", "scvp-vp-request", "spq");
    this->addMimeType("audio", "ogg", "spx");
    this->addMimeType("application", "x-wais-source", "src");
    this->addMimeType("application", "sparql-results+xml", "srx");
    this->addMimeType("application", "vnd.kodak-descriptor", "sse");
    this->addMimeType("application", "vnd.epson.ssf", "ssf");
    this->addMimeType("application", "ssml+xml", "ssml");
    this->addMimeType("application", "vnd.sun.xml.calc.template", "stc");
    this->addMimeType("application", "vnd.sun.xml.draw.template", "std");
    this->addMimeType("application", "vnd.wt.stf", "stf");
    this->addMimeType("application", "vnd.sun.xml.impress.template", "sti");
    this->addMimeType("application", "hyperstudio", "stk");
    this->addMimeType("application", "vnd.ms-pki.stl", "stl");
    this->addMimeType("application", "vnd.pg.format", "str");
    this->addMimeType("application", "vnd.sun.xml.writer.template", "stw");
    this->addMimeType("application", "vnd.sus-calendar", "sus");
    this->addMimeType("application", "vnd.sus-calendar", "susp");
    this->addMimeType("application", "x-sv4cpio", "sv4cpio");
    this->addMimeType("application", "x-sv4crc", "sv4crc");
    this->addMimeType("application", "vnd.svd", "svd");
    this->addMimeType("image", "svg+xml", "svg");
    this->addMimeType("image", "svg+xml", "svgz");
    this->addMimeType("application", "x-director", "swa");
    this->addMimeType("application", "x-shockwave-flash", "swf");
    this->addMimeType("application", "vnd.arastra.swi", "swi");
    this->addMimeType("application", "vnd.sun.xml.calc", "sxc");
    this->addMimeType("application", "vnd.sun.xml.draw", "sxd");
    this->addMimeType("application", "vnd.sun.xml.writer.global", "sxg");
    this->addMimeType("application", "vnd.sun.xml.impress", "sxi");
    this->addMimeType("application", "vnd.sun.xml.math", "sxm");
    this->addMimeType("application", "vnd.sun.xml.writer", "sxw");
    this->addMimeType("text", "troff", "t");
    this->addMimeType("application", "vnd.tao.intent-module-archive", "tao");
    this->addMimeType("application", "x-tar", "tar");
    this->addMimeType("application", "vnd.3gpp2.tcap", "tcap");
    this->addMimeType("application", "x-tcl", "tcl");
    this->addMimeType("application", "vnd.smart.teacher", "teacher");
    this->addMimeType("application", "x-tex", "tex");
    this->addMimeType("application", "x-texinfo", "texi");
    this->addMimeType("application", "x-texinfo", "texinfo");
    this->addMimeType("text", "plain", "text");
    this->addMimeType("application", "x-tex-tfm", "tfm");
    this->addMimeType("application", "x-gzip", "tgz");
    this->addMimeType("image", "tiff", "tif");
    this->addMimeType("image", "tiff", "tiff");
    this->addMimeType("application", "vnd.tmobile-livetv", "tmo");
    this->addMimeType("application", "x-bittorrent", "torrent");
    this->addMimeType("application", "vnd.groove-tool-template", "tpl");
    this->addMimeType("application", "vnd.trid.tpt", "tpt");
    this->addMimeType("text", "troff", "tr");
    this->addMimeType("application", "vnd.trueapp", "tra");
    this->addMimeType("application", "x-msterminal", "trm");
    this->addMimeType("text", "tab-separated-values", "tsv");
    this->addMimeType("application", "x-font-ttf", "ttc");
    this->addMimeType("application", "x-font-ttf", "ttf");
    this->addMimeType("application", "vnd.simtech-mindmapper", "twd");
    this->addMimeType("application", "vnd.simtech-mindmapper", "twds");
    this->addMimeType("application", "vnd.genomatix.tuxedo", "txd");
    this->addMimeType("application", "vnd.mobius.txf", "txf");
    this->addMimeType("text", "plain", "txt");
    this->addMimeType("application", "x-authorware-bin", "u32");
    this->addMimeType("application", "x-debian-package", "udeb");
    this->addMimeType("application", "vnd.ufdl", "ufd");
    this->addMimeType("application", "vnd.ufdl", "ufdl");
    this->addMimeType("application", "vnd.umajin", "umj");
    this->addMimeType("application", "vnd.unity", "unityweb");
    this->addMimeType("application", "vnd.uoml+xml", "uoml");
    this->addMimeType("text", "uri-list", "uri");
    this->addMimeType("text", "uri-list", "uris");
    this->addMimeType("text", "uri-list", "urls");
    this->addMimeType("application", "x-ustar", "ustar");
    this->addMimeType("application", "vnd.uiq.theme", "utz");
    this->addMimeType("text", "x-uuencode", "uu");
    this->addMimeType("application", "x-cdlink", "vcd");
    this->addMimeType("text", "x-vcard", "vcf");
    this->addMimeType("application", "vnd.groove-vcard", "vcg");
    this->addMimeType("text", "x-vcalendar", "vcs");
    this->addMimeType("application", "vnd.vcx", "vcx");
    this->addMimeType("application", "vnd.visionary", "vis");
    this->addMimeType("video", "vnd.vivo", "viv");
    this->addMimeType("application", "vnd.stardivision.writer", "vor");
    this->addMimeType("application", "x-authorware-bin", "vox");
    this->addMimeType("model", "vrml", "vrml");
    this->addMimeType("application", "vnd.visio", "vsd");
    this->addMimeType("application", "vnd.vsf", "vsf");
    this->addMimeType("application", "vnd.visio", "vss");
    this->addMimeType("application", "vnd.visio", "vst");
    this->addMimeType("application", "vnd.visio", "vsw");
    this->addMimeType("model", "vnd.vtu", "vtu");
    this->addMimeType("application", "voicexml+xml", "vxml");
    this->addMimeType("application", "x-director", "w3d");
    this->addMimeType("application", "x-doom", "wad");
    this->addMimeType("audio", "x-wav", "wav");
    this->addMimeType("audio", "x-ms-wax", "wax");
    this->addMimeType("image", "vnd.wap.wbmp", "wbmp");
    this->addMimeType("application", "vnd.criticaltools.wbs+xml", "wbs");
    this->addMimeType("application", "vnd.wap.wbxml", "wbxml");
    this->addMimeType("application", "vnd.ms-works", "wcm");
    this->addMimeType("application", "vnd.ms-works", "wdb");
    this->addMimeType("application", "msword", "wiz");
    this->addMimeType("application", "vnd.ms-works", "wks");
    this->addMimeType("video", "x-ms-wm", "wm");
    this->addMimeType("audio", "x-ms-wma", "wma");
    this->addMimeType("application", "x-ms-wmd", "wmd");
    this->addMimeType("application", "x-msmetafile", "wmf");
    this->addMimeType("text", "vnd.wap.wml", "wml");
    this->addMimeType("application", "vnd.wap.wmlc", "wmlc");
    this->addMimeType("text", "vnd.wap.wmlscript", "wmls");
    this->addMimeType("application", "vnd.wap.wmlscriptc", "wmlsc");
    this->addMimeType("video", "x-ms-wmv", "wmv");
    this->addMimeType("video", "x-ms-wmx", "wmx");
    this->addMimeType("application", "x-ms-wmz", "wmz");
    this->addMimeType("application", "vnd.wordperfect", "wpd");
    this->addMimeType("application", "vnd.ms-wpl", "wpl");
    this->addMimeType("application", "vnd.ms-works", "wps");
    this->addMimeType("application", "vnd.wqd", "wqd");
    this->addMimeType("application", "x-mswrite", "wri");
    this->addMimeType("model", "vrml", "wrl");
    this->addMimeType("application", "wsdl+xml", "wsdl");
    this->addMimeType("application", "wspolicy+xml", "wspolicy");
    this->addMimeType("application", "vnd.webturbo", "wtb");
    this->addMimeType("video", "x-ms-wvx", "wvx");
    this->addMimeType("application", "x-authorware-bin", "x32");
    this->addMimeType("application", "vnd.hzn-3d-crossword", "x3d");
    this->addMimeType("application", "x-silverlight-app", "xap");
    this->addMimeType("application", "vnd.xara", "xar");
    this->addMimeType("application", "x-ms-xbap", "xbap");
    this->addMimeType("application", "vnd.fujixerox.docuworks.binder", "xbd");
    this->addMimeType("image", "x-xbitmap", "xbm");
    this->addMimeType("application", "vnd.syncml.dm+xml", "xdm");
    this->addMimeType("application", "vnd.adobe.xdp+xml", "xdp");
    this->addMimeType("application", "vnd.fujixerox.docuworks", "xdw");
    this->addMimeType("application", "xenc+xml", "xenc");
    this->addMimeType("application", "patch-ops-error+xml", "xer");
    this->addMimeType("application", "vnd.adobe.xfdf", "xfdf");
    this->addMimeType("application", "vnd.xfdl", "xfdl");
    this->addMimeType("application", "xhtml+xml", "xht");
    this->addMimeType("application", "xhtml+xml", "xhtml");
    this->addMimeType("application", "xv+xml", "xhvml");
    this->addMimeType("image", "vnd.xiff", "xif");
    this->addMimeType("application", "vnd.ms-excel", "xla");
    this->addMimeType("application", "vnd.ms-excel.addin.macroenabled.12", "xlam");
    this->addMimeType("application", "vnd.ms-excel", "xlb");
    this->addMimeType("application", "vnd.ms-excel", "xlc");
    this->addMimeType("application", "vnd.ms-excel", "xlm");
    this->addMimeType("application", "vnd.ms-excel", "xls");
    this->addMimeType("application", "vnd.ms-excel.sheet.binary.macroenabled.12", "xlsb");
    this->addMimeType("application", "vnd.ms-excel.sheet.macroenabled.12", "xlsm");
    this->addMimeType("application", "vnd.openxmlformats-officedocument.spreadsheetml.sheet", "xlsx");
    this->addMimeType("application", "vnd.ms-excel", "xlt");
    this->addMimeType("application", "vnd.ms-excel.template.macroenabled.12", "xltm");
    this->addMimeType("application", "vnd.openxmlformats-officedocument.spreadsheetml.template", "xltx");
    this->addMimeType("application", "vnd.ms-excel", "xlw");
    this->addMimeType("application", "xml", "xml");
    this->addMimeType("application", "vnd.olpc-sugar", "xo");
    this->addMimeType("application", "xop+xml", "xop");
    this->addMimeType("application", "xml", "xpdl");
    this->addMimeType("application", "x-xpinstall", "xpi");
    this->addMimeType("image", "x-xpixmap", "xpm");
    this->addMimeType("application", "vnd.is-xpr", "xpr");
    this->addMimeType("application", "vnd.ms-xpsdocument", "xps");
    this->addMimeType("application", "vnd.intercon.formnet", "xpw");
    this->addMimeType("application", "vnd.intercon.formnet", "xpx");
    this->addMimeType("application", "xml", "xsl");
    this->addMimeType("application", "xslt+xml", "xslt");
    this->addMimeType("application", "vnd.syncml+xml", "xsm");
    this->addMimeType("application", "xspf+xml", "xspf");
    this->addMimeType("application", "vnd.mozilla.xul+xml", "xul");
    this->addMimeType("application", "xv+xml", "xvm");
    this->addMimeType("application", "xv+xml", "xvml");
    this->addMimeType("image", "x-xwindowdump", "xwd");
    this->addMimeType("chemical", "x-xyz", "xyz");
    this->addMimeType("application", "vnd.zzazz.deck+xml", "zaz");
    this->addMimeType("application", "zip", "zip");
    this->addMimeType("application", "vnd.zul", "zir");
    this->addMimeType("application", "vnd.zul", "zirz");
    this->addMimeType("application", "vnd.handheld-entertainment+xml", "zmm");
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
        throw MimeType::ExceptionUnknownMimeType();

    return MimeType(this->_map_extensions.at(extension));
}

void MimeTypes::addMimeType(const std::string type, const std::string name, const std::string extension, const bool need_cgi)
{
    this->_map_extensions.insert(std::make_pair(extension, MimeType(name, extension, type, need_cgi)));
}
