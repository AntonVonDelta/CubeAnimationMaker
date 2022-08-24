
var use_between_frames_compression = true;
const compression_block_size_bits = 4;	// How many bits describe the compression block. This sets the max size of a compression block = 2^n-1

// Flattens the planes XcolsXrowsX1bit structure
function flattenPlane(frame) {
	return frame.state.flat().flat();
}

function compress(animation) {
	if (animation.length == 0) return [];

	var compressed_between_frames = [flattenPlane(animation[0])];
	for (var i = 1; i < animation.length; i++) {
		if (use_between_frames_compression) {
			compressed_between_frames.push(compressConsecutiveFrames(flattenPlane(animation[i - 1]), flattenPlane(animation[i])));
		} else {
			compressed_between_frames.push(flattenPlane(animation[i]));
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
		result.push((nr >> i) & 1);
	}
	return result;
}