AUTOMAKE_OPTIONS = foreign

SUBDIRS = 

noinst_PROGRAMS = unittest

unittest_SOURCES = \
	TradeServiceTest.cc \
	MsgServiceTest.cc


unittest_CPPFLAGS = -I$(top_srcdir) -I.. -I..\
		$(BOOST_CPPFLAGS) \
		$(CITICS_CPPFLAGS) \
		$(RAPIDJSON_CPPFLAGS) \
		$(GTEST_CPPFLAGS) \
		$(SOIL_CPPFLAGS) \
		$(JSON_CPPFLAGS)


unittest_LDADD = \
	$(GTEST_LIBADD) \
	../service/libctr.la \
	$(BOOST_LIBADD) \
	$(CITICS_LIBADD) \
	$(SOIL_LIBADD) \
	$(SSL_LIBADD)



unittest_LDFLAGS = $(LIBADD_DL) -Wl,--no-as-needed -pthread

# header files
noinst_HEADERS = 

EXTRA_DIST = env.sh log.cfg trade.cfg msg.cfg


