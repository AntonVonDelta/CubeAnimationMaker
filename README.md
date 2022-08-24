# CubeAnimationMaker

Generator for 3D led cube configurations.
The output is compressed to preserve memory space.

The compression algorithm is as follows:
 - Each frame consists of the entire cube led state (all planes).
 - Every frame is xor substracted from the previous one in order to nullify similar bits. (The first frame is left as it is).
 - On the result it is applied a simple compression:
   - First 4 bits of a compression block encode the length of repeating bits.
   - The next bit represents the repeating value.
   - **If** the 4 bits are equal to 0 then there's no compression and the next bits are read as they are until the end of frame.
 - The constants can be altered easily in code including
   - Length of cube side.
   - Number of bits used for representing compression block size.
   
![image](https://user-images.githubusercontent.com/25268629/186506645-167d0d83-bc1b-41d3-93d7-626565fcd84c.png)
