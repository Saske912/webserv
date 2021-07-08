server {
	host 10.21.31.73									# host address
	port 1024									# port to listen
	server_names hostname.local hostname.test			# server names
	#error_page 404 error_404.html						# error page for 404
	#error_page 401 error_401.html						# error page for 401
    client_max_body_size 100M							# limit client max body sizуe
    allow bla POST
    cgi_path cgi_tester
	route /directory/ {							    # route for file $(index)/public/index.html
       	allowed_methods GET	# allowed methods for route
		root YoupiBanane/
       	index youpi.bad_extension
       	#cgi_extension bla  youpi.bla
    }
    route /put_test/ {
        allowed_methods PUT DELETE
        root YoupiBanane/put_dir/
    }
    route /post_body {
        allowed_methods POST
        root content/
		index index.html
		client_max_body_size 100
    }
	route / {							    # route for file $(index)/public/index.html
		allowed_methods GET					# allowed methods for route
		root content/
		index index.html
	}
}

server {
	host 10.21.31.73								# host address
	port 1025									        # port to listen
	server_names hostname.local hostname.test			# server names
	#error_page 404 error_404.html						# error page for 404
	#error_page 401 error_401.html						# error page for 401
    client_max_body_size 100M							# limit client max body sizуe
    allow bla POST
    cgi_path cgi_tester
	route /directory/ {							    # route for file $(index)/public/index.html
       	allowed_methods GET	# allowed methods for route
		root YoupiBanane/
       	index youpi.bad_extension
       	#cgi_extension bla  youpi.bla
    }
    route /put_test/ {
        allowed_methods PUT
        root YoupiBanane/put_dir/
    }
    route /post_body {
        allowed_methods POST
        root content/
        index tmp.html
        client_max_body_size 100
        }
	route / {							    # route for file $(index)/public/index.html
		allowed_methods GET					# allowed methods for route
		root content/
		index hack.html
	}
}

server {
	host 10.21.31.73										# host address
	port 1026									# port to listen
	server_names hostname.local hostname.test			# server names
	#error_page 404 error_404.html						# error page for 404
	#error_page 401 error_401.html						# error page for 401
	client_max_body_size 100M							# limit client max body size
	route /auto {
	    allowed_methods GET
	    root content
	    directory_listing on
	}
	route /content {
		allowed_methods GET             # route for file $(index)/public/index.html
		root YoupiBanane/				# allowed methods for route
		index youpla.bla
	}
	route /upload/ {									# route for folder
		allowed_methods GET
		directory_listing on							# directory listing aka autoindex			# index files for route
		upload_location /tmp/							# enable uploads and set the save location
		root /Users/pfile/web_c/content							# root folder for route
	}
};
