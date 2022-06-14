#! /usr/bin/python3



f = open("settings.cfg", "w")

f.write("INCLUDE:\t\tinclude\n")
f.write("SDIR:\t\tsrc\n")
f.write("TESTDIR:\t\ttest\n")
f.write("ODIR:\t\tobj\n")
f.write("BIN_NAME:\t\tjosh\n")
f.write("BINPATH:\t\t/usr/local/bin\n")

f.close()