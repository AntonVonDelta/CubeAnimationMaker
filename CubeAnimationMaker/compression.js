const debug = false;

/* 	Algo ideea:
		Get the difference between two frames. Many animations share similar consecutive frames.
		This will reduce all the information in one scene to almost all zeroes.
		This can then be compressed per frame.

	DWORD = 4 bytes = 32bits
		AVR GCC 	unsigned long
		MSVC C++	unsigned int

	Frame data byte format:
		<compression_block_size_bits> bits = Count

		If Count !=0 then block is compressed:
			1 bit = the value of the compressed block
		If Count == 0 then no compression:
			Rest of bits till the end of frame are not compressed

			Reason: no benefit gained from compression (possible if the displayed pattern Chess-like)
			Read the data uncompressed till end of frame.
		<metadata_size_bits> bits = metadata

	Animation data byte format:
		1 dword = frames count
		1 dword = animation metadata
		rest of dwords = frames data

*/

// How many bits describe the compression block. This sets the max size of a compression block = 2^n-1
// Max 8bits. Max compression block 255bits (we loose the value 0)
const compression_block_size_bits = 4;

// How many bits are used to describe metadata
// Max 8bits.
const metadata_size_bits = 5;

// Adds duration metadata or anything else to the compressed stream
function compressWithMetadata(animation) {
	var result = [];
	var compressed_binary = compress(animation);
	var bits_offset = 0;

	for (var i = 0; i < animation.frames.length; i++) {
		// Add metadata to compressed frame
		var metadata = animation.frames[i].duration;
		result.push(compressed_binary[i].concat(intToBitArray(metadata, metadata_size_bits)));

		if (debug) {
			console.log("Frame " + i + "; starts: " + bits_offset + "; size: " + result.at(-1).length);
			bits_offset += result.at(-1).length;
		}
	}

	// Add metadata to animation
	return intToBitArray(animation.frames.length, 32).concat(result);
}

// Compress all animation frames
// Returns 2D array for each frame of binary data
function compress(animation) {
	if (animation.frames.length == 0) return [];

	var compressed_between_frames = [flattenPlane(animation.frames[0])];
	for (var i = 1; i < animation.frames.length; i++) {
		if (animation.use_frames_subtraction) {
			compressed_between_frames.push(compressConsecutiveFrames(flattenPlane(animation.frames[i - 1]), flattenPlane(animation.frames[i])));
		} else {
			compressed_between_frames.push(flattenPlane(animation.frames[i]));
		}
	}

	var compressed_frames = [];
	for (var i = 0; i < compressed_between_frames.length; i++) {
		compressed_frames.push(compressFrame(compressed_between_frames[i]));
	}
	return compressed_frames;
}

// Compress frame by xor method
// Returns flat array
function compressConsecutiveFrames(frame1, frame2) {
	// prev_frame x next_frame = diff_bit  | diff_bit x prev_frame = next_frame
	// 1 x 1 = 0  | 0 x 1 = 1
	// 0 x 1 = 1  | 1 x 0 = 1
	// 1 x 0 = 1  | 1 x 1 = 0
	// 0 x 0 = 0  | 0 x 0 = 0
	return frame2.map((el, index) => el ^ frame1[index]);
}

// Returns compressed array
function compressFrame(frame_flat_array) {
	// Use backtracking to get optimum compression result

	if (frame_flat_array.length == 0) return [];

	var count_similar_bits = 1;
	var first_bit = frame_flat_array[0];
	for (var i = 1; i < frame_flat_array.length; i++) {
		if (count_similar_bits == Math.pow(2, compression_block_size_bits) - 1) break;
		if (frame_flat_array[i] == first_bit) {
			count_similar_bits += 1;
		} else break;
	}

	var next_compression_part = compressFrame(frame_flat_array.slice(count_similar_bits));
	var size_compressed = 4 + 1 + next_compression_part.length;
	var size_uncompressed = 4 + frame_flat_array.length;

	if (size_compressed < size_uncompressed) {
		var block_header = intToBitArray(count_similar_bits, 4);
		block_header.push(first_bit);
		return block_header.concat(next_compression_part);
	}
	return intToBitArray(0, 4).concat(frame_flat_array);
}

// Returns n bits
function intToBitArray(nr, bits) {
	var result = [];

	if (nr < 0 || nr > Math.pow(2, bits) - 1) throw "Number too large";

	for (var i = 0; i < bits; i++) {
		result.push((nr >>> i) & 1);
	}
	return result;
}

// Returns a DWORD condensed array
// Little endian
function condenseBinary(arr) {
	var result = [];

	for (var i = 0; i < Math.floor(arr.length / 32); i++) {
		var dword = 0;
		for (var j = 0; j < 32; j++) {
			dword += arr[i * 32 + j] * ((1 << j) >>> 0);
		}
		result.push(dword);
	}

	// Remaining bits
	if (arr.length % 32 != 0) {
		var dword = 0;
		for (var j = 0; j < 32; j++) {
			var bit_id = Math.floor(arr.length / 32) * 32 + j;
			if (bit_id >= arr.length) continue;
			dword += arr[bit_id] * ((1 << j) >>> 0);
		}
		result.push(dword);
	}
	return result;
}

// Flattens the planes XcolsXrowsX1bit structure
function flattenPlane(frame) {
	return frame.state.flat().flat();
}

function compressionRatio(animation, compressed_binary) {
	// A good algorithm for displaying the cube would use the same ideea as I did.
	// It would compactify the numbers into dword numbers to preserve instruction calls.
	var sides = animation.cube_side;
	var estimate_binary_uncompressed = animation.frames.length * (sides * sides * sides + metadata_size_bits);
	var estimate_bytes_uncompressed = Math.ceil(estimate_binary_uncompressed / 8);
	var compressed_bytes = Math.ceil(compressed_binary.length / 8);
	return (compressed_bytes / estimate_bytes_uncompressed * 100).toFixed(2);
}

function compressionRatioLCCG(animation, compressed_binary) {
	// LCCG is very ineficient. It uses 8bit to store just 4bits at a time.
	var sides = animation.cube_side;
	var estimate_binary_uncompressed = animation.frames.length * (sides * sides * 8 + 8);
	var estimate_bytes_uncompressed = Math.ceil(estimate_binary_uncompressed / 8);
	var compressed_bytes = Math.ceil(compressed_binary.length / 8);
	return (compressed_bytes / estimate_bytes_uncompressed * 100).toFixed(2);
}