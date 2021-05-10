#ifndef HEADER_HPP
#define HEADER_HPP

class Header
{
	public:
		std::string &getAccept_Charsets();
		std::string &getAccept_Language();
		std::string &getAllow();
		std::string &getAuthorization();
		std::string &getContent_Language();
		std::string &getContent_Length();
		std::string &getContent_Location();
		std::string &getContent_Type();
		std::string &getDate();
		std::string &getHost();
		std::string &getHttp();
		std::string &getLast_Modified();
		std::string &getLocation();
		std::string &getMethod();
		std::string &getReferer();
		std::string &getRequest();
		std::string &getRetry_after();
		std::string &getServer();
		std::string &getTransfer_Encoding();
		std::string &getUser_Agent();
		std::string &getWWW_Authenticate();
		void setAccept_Charsets(std::string const &);
		void setAccept_Language(std::string const &);
		void setAllow(std::string const &);
		void setAuthorization(std::string const &);
		void setContent_Language(std::string const &);
		void setContent_Length(std::string const &);
		void setContent_Location(std::string const &);
		void setContent_Type(std::string const &);
		void setDate(std::string const &);
		void setHost(std::string const &);
		void setHttp(std::string const &);
		void setLast_Modified(std::string const &);
		void setLocation(std::string const &);
		void setMethod(std::string const &);
		void setReferer(std::string const &);
		void setRequest(std::string const &);
		void setRetry_after(std::string const &);
		void setServer(std::string const &);
		void setTransfer_Encoding(std::string const &);
		void setUser_Agent(std::string const &);
		void setWWW_Authenticate(std::string const &);
		void eraseStruct();
	private:
		std::string Request;
		std::string Method;
		std::string Http;
		std::string Accept_Charsets;
		std::string Accept_Language;
		std::string Allow;
		std::string Authorization;
		std::string Content_Language;
		std::string Content_Length;
		std::string Content_Location;
		std::string Content_Type;
		std::string Date;
		std::string Host;
		std::string Last_Modified;
		std::string Location;
		std::string Referer;
		std::string Retry_after;
		std::string Server;
		std::string Transfer_Encoding;
		std::string User_Agent;
		std::string WWW_Authenticate;
};

#endif
