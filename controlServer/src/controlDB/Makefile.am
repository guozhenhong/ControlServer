INCLUDES = -I$(top_srcdir)/include -I/usr/local/sqlite3/include -I$(top_srcdir)/../lab_common/trunk/include
noinst_LTLIBRARIES=libdb.la
libdb_la_SOURCES = CppSQLite3.cpp \
				   CServerInfoTable.cpp \
				   CCtrlDBHandler.cpp \
				   CTableHandler.cpp \
				   CStorageUnitInfoTable.cpp \
				   CVirtualDiskInfoTable.cpp
libdb_la_LIBADD = $(top_srcdir)/../lab_common/trunk/lib/libcommon.la
