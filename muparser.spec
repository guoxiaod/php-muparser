#
# spec file for package muparser
#
# Copyright (c) 2013 SUSE LINUX Products GmbH, Nuernberg, Germany.
#
# All modifications and additions to the file contributed by third parties
# remain the property of their copyright owners, unless otherwise agreed
# upon. The license for this file, and modifications and additions to the
# file, is the same license as for the pristine package itself (unless the
# license for the pristine package is not an Open Source License, in which
# case the license is the MIT License). An "Open Source License" is a
# license that conforms to the Open Source Definition (Version 1.9)
# published by the Open Source Initiative.

# Please submit bugfixes or comments via http://bugs.opensuse.org/
#


Name:           muparser
%define lname   libmuparser2
Summary:        A math parser library
License:        MIT
Group:          Productivity/Scientific/Math
Version:        2.2.4
Release:        0
Url:            http://muparser.beltoforion.de/

#DL-URL:    http://downloads.sf.net/muparser/muparser_v2_2_3.zip
# quilt does not like zips very much. downloaded and repackaged.
Source:         %name-%version.tgz
#Source9:        prepare-tar.sh
#Patch3:         muparser-optflags.patch
Patch8:          muparser-utf8.patch
BuildRoot:      %{_tmppath}/%{name}-%{version}-build
BuildRequires:  autoconf
BuildRequires:  automake
BuildRequires:  gcc-c++
BuildRequires:  libtool
BuildRequires:  pkgconfig
#BuildRequires:  xz

%description
muParser is an extensible high performance math parser library written in
C++. It works by transforming a mathematical expression into bytecode and
precalculating constant parts of the expression.

%package -n %lname
Summary:        Library to evaluate strings as mathematical functions
Group:          System/Libraries

%description -n %lname
muParser is an extensible high performance math parser library written in
C++. It works by transforming a mathematical expression into bytecode and
precalculating constant parts of the expression.

%package devel
Summary:        Development files for muparser
Group:          Development/Libraries/C and C++
Requires:       %lname = %version

%description devel
muParser is an extensible high performance math parser library written in
C++. It works by transforming a mathematical expression into bytecode and
precalculating constant parts of the expression.

%prep
%setup -q
#%patch -P 3 -p1
%patch -P 8 -p0

%build
sh build/autoconf/acregen.sh
%configure --enable-samples --enable-shared
# bakafile not parallel sife - duh
make -j1

%install
make install DESTDIR="%buildroot"
rm -f "%buildroot/%_libdir"/*.la

%post -n %lname -p /sbin/ldconfig

%postun -n %lname -p /sbin/ldconfig

%files -n %lname
%defattr(-,root,root)
%_libdir/libmuparser.so.2*
%doc License.txt

%files devel
%defattr(-,root,root)
%_includedir/muParser*.h
%_libdir/libmuparser.so
%_libdir/pkgconfig/muparser.pc
%doc Changes.txt

%changelog
