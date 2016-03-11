AUTOMAKE_OPTIONS = foreign

bin_PROGRAMS = trade_test
trade_test_SOURCES = \
	TradeTest.cc \
	Main.cc

trade_test_CPPFLAGS = -I$(top_srcdir) \
		$(BOOST_CPPFLAGS) \
		$(SOIL_CPPFLAGS) \
		$(CITICS_CPPFLAGS)



trade_test_LDADD = \
	$(BOOST_LIBADD) \
	$(SOIL_LIBADD) \
	../service/libctr.la \
	$(CITICS_LIBADD) \
	$(SSL_LIBADD)

trade_test_LDFLAGS = $(LIBADD_DL) -Wl,--no-as-needed -pthread

# header files
noinst_HEADERS = TradeTest.hh

EXTRA_DIST = env.sh trade.cfg log.cfg

trade_testdir = $(bindir)
trade_test_DATA = $(EXTRA_DIST)

