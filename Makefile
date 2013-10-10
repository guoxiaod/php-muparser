include ../../../rules/base.mk


NAME=muparser
VERSION=2.2.4
SRC_FILE=${NAME}-${VERSION}.tgz

all: checkout-muparser build-muparser copy
	

checkout-muparser:
	if [ ! -e ${NAME} ]; then\
		svn checkout http://muparser.googlecode.com/svn/trunk ${NAME}; \
	fi

build-muparser:
	cp -r ${NAME} ${NAME}-${VERSION}
	tar -czf ${SRC_FILE} ${NAME}-${VERSION}
	cp ${SRC_FILE} ${RPMBUILD_ROOT}/SOURCES/
	cp muparser-utf8.patch ${RPMBUILD_ROOT}/SOURCES/
	${RPMBUILD_SPEC} ${NAME}.spec

copy:
	cp ${RPMBUILD_ROOT}/RPMS/${ARCH}/*${NAME}*${VERSION}*.rpm .

build-ext:
	rm -rf php-${NAME}-${VERSION}*
	cp -r php-${NAME} php-${NAME}-${VERSION}
	tar -czf php-${NAME}-${VERSION}.tgz php-${NAME}-${VERSION}
	cp php-${NAME}-${VERSION}.tgz ${RPMBUILD_ROOT}/SOURCES/
	${RPMBUILD_SPEC} php-${NAME}.spec
	rm -rf php-${NAME}-${VERSION}*

copy-ext:
	cp ${RPMBUILD_ROOT}/RPMS/${ARCH}/php-${NAME}*${VERSION}*.rpm .
	
