1. Generate private certificate authority key.

`openssl genrsa -aes256 -passout pass:haslo -out ca.key 4096`

2. Generate CA certificate (self signed)

`openssl req -new -x509 -days 3650 -key ca.key -out ca.crt`

3. Generate private key for service

`openssl genrsa -out radegast_service.key 4096`

4. Modify opensssl.cnf to add subjectAltName [more info](http://apetec.com/support/generatesan-csr.htm)

`cp /etc/ssl/openssl.cnf myopensssl.cnf`

changes:

    1. uncomment line: req_extensions = v3_req

    2. add: subjectAltName = @alt_names in [ v3_req] section
    
    3. add [alt_names] section:
        [alt_names]
    
        DNS.1 = www.radegast.store
        
        DNS.2 = mail.radegast.store
        
        DNS.3 = radegast.store # Firefox doesn't verify domain names from common name

5. Create request for certificate (fill "radegast.store" in common name field)

`openssl req -new -key radegast_service.key -out radegast_service.csr -config myopenssl.cnf`

6. Create certificate by CA

`openssl x509 -req -in radegast_service.csr -CA ca.crt -CAkey ca.key -CAcreateserial -out radegast_service.crt -days 730 -extensions v3_req -extfile myopenssl.cnf`

7. Create docker images

`docker build -t radegast .`

8. Run container in detached mode

`docker run -d radegast`

9. Check container_id and container_ip

`docker container ls
docker inspect -f '{{range .NetworkSettings.Networks}}{{.IPAddress}}{{end}}' container_id`

10. Add line to /etc/hosts at host machine

`container_ip radegast.store www.radegast.store mail.radegast.store`

11. Import ca.crt to your browser
