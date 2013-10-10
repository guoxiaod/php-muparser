# $Id: php-pecl-http.spec 7 2009-09-11 10:34:42Z preilly@bigtent.com $
# yum install -y libssh2-devel openssl-devel libidn-devel openldap-devel c-ares-devel gnutls-devel curl-devel php-devel
%define php_extdir %(php-config --extension-dir 2>/dev/null || echo %{_libdir}/php4)
%define php_config_opts %(php-config --configuration-options 2>/dev/null)
%define _dbg %{nil}
%define peclname %{lua:

name = "php-muparser"

php_config_opts = rpm.expand("%{php_config_opts}")
if (string.find(php_config_opts, "--enable-debug", 1, true)) then
  name = "php-dbg" .. string.sub(name, 4)
end
print(name)
}
%{lua: 
current_name = rpm.expand("%{peclname}")
if (string.find(current_name, "-dbg", 1, true)) then
  rpm.define("_dbg -dbg")
end
}

Summary: PECL package for muparser
Name: %{peclname}
Version: 2.2.4
Release: 1
License: PHP
Group: Development/Languages
URL: http://pecl.php.net/package/%{name}
Source: http://pecl.php.net/get/%{name}-%{version}.tgz
BuildRoot: %{_tmppath}/%{name}-%{version}-%{release}-root
Requires: php%{_dbg} > 5.4, libstdc++, libmuparser2
BuildRequires: php%{_dbg}, php%{_dbg}-devel,libstdc++-devel,libtool, muparser-devel
# Required by phpize
BuildRequires: autoconf, automake, libtool
BuildArch: x86_64

%description
Yongche geo php ext

%prep
%setup -n %{name}-%{version}

%build
# Workaround for broken old phpize on 64 bits
%{__cat} %{_bindir}/phpize | sed 's|/lib/|/%{_lib}/|g' > phpize && sh phpize
%configure 
%{__make} %{?_smp_mflags}

%{__make} test

%install
%{__rm} -rf %{buildroot}
%{__make} install INSTALL_ROOT=%{buildroot}

# Drop in the bit of configuration
%{__mkdir_p} %{buildroot}%{_sysconfdir}/php.d

%{__cat} > %{buildroot}%{_sysconfdir}/php.d/muparser.ini << 'EOF'
; Enable muparser extension
extension=muparser.so
EOF

%clean
%{__rm} -rf %{buildroot}

%post 

%files
%defattr(-, root, root, 0755)
%config(noreplace) %{_sysconfdir}/php.d/muparser.ini
%{php_extdir}/muparser.so

%changelog
* Thu Oct 10 2013 Anders <gxd305@gmail.com> 2.2.4
- init version
