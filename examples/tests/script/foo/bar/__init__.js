print("importing " + __extension__);
__setupPackage__("foo.bar");

foo.bar.add = function(a, b) { return a + b; }
foo.bar.subtract = function(a, b) { return a - b; }