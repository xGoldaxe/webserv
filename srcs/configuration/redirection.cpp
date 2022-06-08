#include "redirection.hpp"

Redirection::Redirection(int redirect_code, std::string url, std::string redirect_url)
{
	this->redirect_code = redirect_code;
	this->url = url;
	this->redirect_url = redirect_url;
}

int Redirection::get_redirect_code() const
{
	return this->redirect_code;
}
std::string Redirection::get_url() const
{
	return this->url;
}
std::string Redirection::get_redirect_url() const
{
	return this->redirect_url;
}
void Redirection::add_redirect_code(int redirect_code)
{
	this->redirect_code = redirect_code;
}
void Redirection::add_url(std::string url)
{
	this->url.clear();
	this->url.append(url);
}
void Redirection::add_redirect_url(std::string redirect_url)
{
	this->redirect_url.clear();
	this->redirect_url.append(redirect_url);
}
