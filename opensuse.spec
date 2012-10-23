%define realname ekdtree
%define revision fbf7957f3309082007294f63cd4b142a4e01dd31
%define install_dir %{_libdir}/erlang/lib/%{realname}-%{version}

Name:          strikead-erlang-%{realname}
Version:       0.1.0
Release:       5
License:       Proprietary
Vendor:        StrikeAd
Packager:      Aleksey Morarash <aleksey.morarash@strikead.com>
Group:         Development/Languages/Erlang
Source:        strikead-%{realname}.tar.gz
Summary:       Erlang interface to CGAL k-d tree and k-NN search algorithm.
URL:           https://github.com/strikead/%{realname}/tarball/%{revision}
Requires:      erlang libcgal9 >= 4.0.2-8
BuildRequires: erlang erlang-rebar gcc-c++ libcgal-devel >= 4.0.2-8
BuildArch:     x86_64

%description
Erlang interface to CGAL k-d tree and k-NN search algorithm.

%prep
%setup -n strikead-%{realname}-%{revision}

%build
rebar compile

%install
install --mode=755 --directory %{buildroot}%{install_dir}/ebin
install --mode=644 ebin/*.beam ebin/*.app %{buildroot}%{install_dir}/ebin
install --mode=755 --directory %{buildroot}%{install_dir}/priv
install --mode=755 priv/%{realname}.so %{buildroot}%{install_dir}/priv
install --mode=755 --directory %{buildroot}%{_docdir}/%{name}
install --mode=644 README.md %{buildroot}%{_docdir}/%{name}

%files
%defattr(-,root,root)
%dir %{install_dir}
%dir %{install_dir}/ebin
%dir %{install_dir}/priv
%dir %{_docdir}/%{name}
%{install_dir}/ebin/*
%{install_dir}/priv/*
%{_docdir}/%{name}/*

%changelog
* Fri Oct 19 2012 Aleksey Morarash <aleksey.morarash@strikead.com> 0.1.0-1
- Initial package for OpenSUSE.

