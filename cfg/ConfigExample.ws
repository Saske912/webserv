server {
	host 10.21.31.71										# host address
	port 1024									# port to listen
	server_names hostname.local hostname.test			# server names
	#error_page 404 error_404.html						# error page for 404
	#error_page 401 error_401.html						# error page for 401
	client_max_body_size 100M							# limit client max body size
	route /content/ {							    # route for file $(index)/public/index.html
		allowed_methods GET POST				# allowed methods for route
		root /content
		index /index.html
	}
	route /upload/ {									# route for folder
		allowed_methods GET
		directory_listing on							# directory listing aka autoindex			# index files for route
		upload_location /tmp/							# enable uploads and set the save location
		root /Users/pfile/web_c/content							# root folder for route
	}
};
