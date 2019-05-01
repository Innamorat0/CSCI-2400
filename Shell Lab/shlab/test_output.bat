#/!bin/bash
make

diff -y <(make rtest01) <(make test01)
diff -y <(make rtest02) <(make test02)
diff -y <(make rtest03) <(make test03)
diff -y <(make rtest04) <(make test04)
diff -y <(make rtest05) <(make test05)
diff -y <(make rtest06) <(make test06)
diff -y <(make rtest07) <(make test07)
diff -y <(make rtest08) <(make test08)
diff -y <(make rtest09) <(make test09)
diff -y <(make rtest10) <(make test10)
diff -y <(make rtest11) <(make test11)
diff -y <(make rtest12) <(make test12)
diff -y <(make rtest13) <(make test13)
diff -y <(make rtest14) <(make test14)
diff -y <(make rtest15) <(make test15)
diff -y <(make rtest16) <(make test16)