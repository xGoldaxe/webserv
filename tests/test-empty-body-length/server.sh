docker rm nginx-test-server
docker run --name nginx-test-server \
            -v "/mnt/d/Programmation/Docker/webserv/test-empty-body-length/nginx.conf:/etc/nginx/nginx.conf:ro" \
            -v "/mnt/d/Programmation/Docker/webserv/test-empty-body-length/static:/www/data/" \
            -p 8080:80 \
            nginx
