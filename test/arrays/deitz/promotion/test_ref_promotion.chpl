class C {
  var A: [1..5] int;
}

var c = C();
var B: [1..5] int;

c.A = 1..5;
B = 6..10;

writeln(B*c.A);
