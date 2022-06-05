x = [[2,0],[0,1]];
y = $x+$x;
z = gem($x*$y);
det($z);
rank($z);
inv($z);
build/matice.txt < $z;
x = @build/matice.txt;
sel(join(@build/matice.txt, $z), [[0,0],[1,1]]);
