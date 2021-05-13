server {
	host 10.21.31.73										# host address
	port 1024									# port to listen
	server_names hostname.local hostname.test			# server names
	#error_page 404 error_404.html						# error page for 404
	#error_page 401 error_401.html						# error page for 401
	client_max_body_size 100M							# limit client max body size
#	root /Users/wpersimm/webserv									# default root for server
	route /kek/ {							    # route for file $(index)/public/index.html
		allowed_methods GET POST					# allowed methods for route
		root /Users/wpersimm/webserv/
		index index.html
	}
	route /upload/ {									# route for folder
		allowed_methods GET
		directory_listing on							# directory listing aka autoindex
		index index.jpg index.png index.html			# index files for route
		upload_location /tmp/							# enable uploads and set the save location
		root /var/www/upload							# root folder for route
	}
	route /special {
		cgi_extension php /bin/fastcgi.app				# cgi file for specified extension
	}
}
server {
	host hostname; port 127  		; ;
	;;	server_names hostname.local hostname.test;	error_page 404 error_404.html;
	;error_page 401 error_401.html;	client_max_body_size 104857600;
};
