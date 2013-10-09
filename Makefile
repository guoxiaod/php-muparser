


all: checkout-muparser patch
	

checkout-muparser:
	if [ ! -e muparser-read-only ]; \
		svn checkout http://muparser.googlecode.com/svn/trunk muparser-read-only; \
	fi

patch:
	cd muparser-read-only && patch -p0 < ../muparser-utf8.patch
