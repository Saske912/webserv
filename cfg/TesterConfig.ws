server {
	host 10.21.31.71										# host address
	port 1024									# port to listen
	server_names hostname.local hostname.test			# server names
	#error_page 404 error_404.html						# error page for 404
	#error_page 401 error_401.html						# error page for 401
	client_max_body_size 100M							# limit client max body size
	route / {							    # route for file $(index)/public/index.html
		allowed_methods GET	POST		# allowed methods for route
		root /Users/pfile/web4/content/
		index index.html
	}
	route / {
	    allowed_methods POST
	    cgi_extension bla cgi_test
	}
	route /put_test/ {
	    allowed_methods PUT
	}
	route /post_body {
        client_max_body_size 100
	}
	route /directory/ {
        allowed_methods GET
        root /Users/pfile/web4/content/YoupiBanane
        index youpi.bad_extension
	}
}
