server {
	host 10.21.33.13										# host address
	port 1024									# port to listen
	server_names hostname.local hostname.test			# server names
	#error_page 404 error_404.html						# error page for 404
	#error_page 401 error_401.html						# error page for 401
    client_max_body_size 100M							# limit client max body sizуe
    allow bla POST
    cgi_path cgi_tester
	route /directory/ {							    # route for file $(index)/public/index.html
       	allowed_methods GET	# allowed methods for route
		root /Users/pfile/web_c/YoupiBanane/
       	index youpi.bad_extension
       	#cgi_extension bla  youpi.bla
    }
    route /put_test/ {
        allowed_methods PUT
        root /Users/pfile/web_c/YoupiBanane/put_dir/
    }
    route /post_body {
        allowed_methods POST
        root /Users/pfile/web_c/content/
        index tmp.html
        client_max_body_size 100
        }
	route / {							    # route for file $(index)/public/index.html
		allowed_methods GET					# allowed methods for route
		root /Users/pfile/web_c/content/
		index hack.html
	}
}
