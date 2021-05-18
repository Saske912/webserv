server {
	host 10.21.31.71										# host address
	port 1024									# port to listen
	server_names hostname.local hostname.test			# server names
	#error_page 404 error_404.html						# error page for 404
	#error_page 401 error_401.html						# error page for 401
	client_max_body_size 100M							# limit client max body size
	route /directory/ {							    # route for file $(index)/public/index.html
       	allowed_methods GET		# allowed methods for route
       	root /Users/pfile/web4/YoupiBanane/
       	index youpi.bad_extension
    }
    route /put_test/ {
        allowed_methods PUT
        root /Users/pfile/web4/YoupiBanane/put_dir/
        index /
    }
	route / {							    # route for file $(index)/public/index.html
		allowed_methods GET		# allowed methods for route
		root /Users/pfile/web4/content/
		index index.html
	}
}
