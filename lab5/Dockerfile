FROM debian:buster

MAINTAINER Andrzej Zapała

RUN apt-get update && apt-get install -y nginx

COPY ./html/radegast.store /var/www/radegast.store/html
COPY ./html/mail.radegast.store /var/www/mail.radegast.store/html


COPY ./certifications/radegast_service.crt  /etc/ssl/certs/ 
COPY ./certifications/radegast_service.key /etc/ssl/private/

COPY ./conf/default /etc/nginx/sites-available/
COPY ./conf/mail.radegast.store /etc/nginx/sites-available/

RUN ln -sf /etc/nginx/sites-available/default /etc/nginx/sites-enabled/ && ln -sf /etc/nginx/sites-available/mail.radegast.store /etc/nginx/sites-enabled/

EXPOSE 80 443
CMD ["nginx", "-g", "daemon off;"]
