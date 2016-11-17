
#!/bin/bash

cd outs

for i in *.txt ; do
	
	../matrix-rasterize/Debug/mraster $i 20 20

done
