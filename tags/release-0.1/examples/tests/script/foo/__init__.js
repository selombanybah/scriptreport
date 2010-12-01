print("importing " + __extension__);
__setupPackage__("foo");

foo.add = function(a, b) { return a + b; }
foo.subtract = function(a, b) { return a - b; }