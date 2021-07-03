server {
	host 127.0.0.1										# host address
	port 1024									# port to listen
	server_names hostname.local hostname.test			# server names
	#error_page 404 error_404.html						# error page for 404
	#error_page 401 error_401.html						# error page for 401
	client_max_body_size 100M							# limit client max body size
	route /auto {
	    allowed_methods GET
	    root content
	    directory_listing on
	}
	route /content/ {
		allowed_methods GET             # route for file $(index)/public/index.html
		root YoupiBanane/				# allowed methods for route
		index /youpi.bla
	}
	route /upload/ {									# route for folder
		allowed_methods GET
		directory_listing on							# directory listing aka autoindex			# index files for route
		upload_location /tmp/							# enable uploads and set the save location
		root /Users/pfile/web_c/content							# root folder for route
	}
};
