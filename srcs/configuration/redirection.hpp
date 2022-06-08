#pragma once

#include <string>

class Redirection
{

	private:
		int redirect_code;
		std::string url;
		std::string redirect_url;
	public:
		Redirection(int redirect_code, std::string url, std::string redirect_url);

		int								get_redirect_code() const;
		std::string						get_url() const;
		std::string						get_redirect_url() const;
		void							add_redirect_code(int redirect_code);
		void 							add_url(std::string url);
		void 							add_redirect_url(std::string redirect_url);
};
