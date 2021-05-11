server {
	host hostname										# host address
	port 127											# port to listen
	server_names hostname.local hostname.test			# server names
	error_page 404 error_404.html						# error page for 404
	error_page 401 error_401.html						# error page for 401
	client_max_body_size 100M							# limit client max body size
	root /var/www/										# default root for server
	route /public/index.html {							# route for file $(index)/public/index.html
		allowed_methods GET POST						# allowed methods for route
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
