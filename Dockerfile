FROM garland/xampp-base
MAINTAINER steghofe@fim.uni-passau.de

RUN sed -i'' '61,64 s/^/#/' /opt/lampp/etc/extra/httpd-xampp.conf

#TODO /opt/lampp/lampp security #(set all passwords to softsec and allow remote access to mysql)
#TODO set config variables

RUN git clone https://github.com/felixsteghofer/seclab_webapps.git

RUN mv seclab_webapps/vbank /opt/lampp/htdocs/vbank
RUN mv seclab_webapps/dvwa /opt/lampp/htdocs/dvwa

#hack for not having to edit the apache config but to be able to commit the edited files in the htdocs folder
RUN ln -s /opt/lampp/htdocs/vbank seclab_webapps/vbank
RUN ln -s /opt/lampp/htdocs/dvwa seclab_webapps/dvwa 

RUN chmod 777 -R /opt/lampp/htdocs/

EXPOSE 80
EXPOSE 433
EXPOSE 21

WORKDIR /opt/lampp/htdocs

RUN /opt/lampp/lampp start

CMD ["/bin/bash"]

#sudo docker run -t -i -p 8080:80 -p 2121:21 garland/xampp-base /bin/bash
