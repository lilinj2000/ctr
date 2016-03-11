AUTOMAKE_OPTIONS = foreign

bin_PROGRAMS = msg_test
msg_test_SOURCES = \
	MsgTest.cc \
	Main.cc

msg_test_CPPFLAGS = -I$(top_srcdir) \
		$(BOOST_CPPFLAGS) \
		$(SOIL_CPPFLAGS) \
		$(CITICS_CPPFLAGS) \
		$(RAPIDJSON_CPPFLAGS) \
		$(JSON_CPPFLAGS)



msg_test_LDADD = \
	$(BOOST_LIBADD) \
	$(SOIL_LIBADD) \
	../service/libctr.la \
	$(CITICS_LIBADD) \
	$(SSL_LIBADD)

msg_test_LDFLAGS = $(LIBADD_DL) -Wl,--no-as-needed -pthread

# header files
noinst_HEADERS = MsgTest.hh

EXTRA_DIST = env.sh msg.cfg log.cfg

msg_testdir = $(bindir)
msg_test_DATA = $(EXTRA_DIST)

