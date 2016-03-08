AUTOMAKE_OPTIONS = foreign

lib_LTLIBRARIES = libctr.la
libctr_la_SOURCES = CtrOptions.cc CtrServiceImpl.cc

libctr_la_CPPFLAGS = -I$(top_srcdir) \
			$(BOOST_CPPFLAGS) \
			$(CITICS_CPPFLAGS) \
			$(SOIL_CPPFLAGS)

libctr_la_LIBADD = $(CITICS_LIBADD) \
		$(BOOST_LIBADD) \
		$(SOIL_LIBADD) \
		$(SSL_LIBADD)

libctr_la_LDFLAGS = \
	-no-undefined -version-info \
	$(LT_CURRENT):$(LT_REVISION):$(LT_AGE) \
	-release $(LT_RELEASE)

# header files
noinst_HEADERS = CtrLog.hh CtrOptions.hh CtrServiceImpl.hh CtrDef.hh



