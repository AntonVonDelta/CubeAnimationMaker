var frame_cursor = 0;			// Slider cursor: 0 means begining of animation
var animation = {
	use_frames_subtraction: true,
	use_block_compression: true,
	default_duration: 1,		// This is the unit-time. It will be multiplied in arduino code by the true standard frame duration
	cube_side: 4,
	frames: []
};


$(document).ready(function () {
	loadAnimation();
	constructPage(animation);

	$(".grid_cell").click(function (e) {
		$(this).button("toggle");
		console.log($(this).attr("data-col"));
	});
	$("#clear_all").click(function () {
		$(".grid_cell").removeClass("active");
	});
	$("#set_all").click(function () {
		$(".grid_cell").addClass("active");
	});



	$("#insert_frame").click(function () {
		var frame = getFrame(animation.cube_side);
		animation.frames.splice(frame_cursor, 0, frame);
		console.log(frame);

		frame_cursor += 1;
		updateUI();
	});
	$("#update_frame").click(function () {
		if (isNewFrame()) {
			alert("Frame #" + frame_id + " doesn't exist");
			return;
		}
		animation.frames.splice(frame_cursor, 1, getFrame(animation.cube_side));
		updateUI(animation.cube_side);
	});
	$("#refresh_frame").click(function () {
		updateUI(animation.cube_side);
	});
	$("#delete_frame").click(function () {
		if (isNewFrame()) {
			return;
		}
		animation.frames.splice(frame_cursor, 1);
		updateUI(animation.cube_side);
	});
	$("#seek_frame").click(function () {
		var frame_id = parseInt($("#seek_frame_id").val());
		if (isNewFrame(frame_id)) {
			alert("Frame #" + frame_id + " doesn't exist");
			return;
		}
		frame_cursor = frame_id;
		updateUI(animation.cube_side);
	});



	$("#frame_range").on("input change", function () {
		var new_value = parseInt($(this).val());
		frame_cursor = new_value;
		updateUI(animation.cube_side);
	});



	$("#generate_compress").click(function () {
		var compressed_binary = compressWithMetadata(animation).flat();
		var condensed_dwords = condenseBinary(compressed_binary);
		var formatted_list = condensed_dwords.map(x => x + "UL");
		var result = "const unsigned long PROGMEM animation[]={" + formatted_list.join() + "};";

		console.log("Compressed binary", compressed_binary);

		$("#output").val(result);
		$("#compression_ratio").text(compressionRatio(animation, compressed_binary) + "%");
		$("#compression_ratio_lccg").text(compressionRatioLCCG(animation, compressed_binary) + "%");
	});
	$("#settings").click(function () {
		$("#settings_frame_subtraction").prop("checked", animation.use_frames_subtraction);
		$("#settings_block_compression").prop("checked", animation.use_block_compression);
		$("#settings_default_duration").val(animation.default_duration);
		$("#settings_cube_side").val(animation.cube_side);

		$("#settings_modal").modal("show");
	});
	$("#settings_modal_save").click(function () {
		var new_frame_subtraction = $("#settings_frame_subtraction").is(':checked');
		var new_block_compression = $("#settings_block_compression").is(':checked');
		var new_default_duration = parseInt($("#settings_default_duration").val());
		var new_cube_sides = parseInt($("#settings_cube_side").val());

		var old_frame_subtraction = animation.use_frames_subtraction;
		var old_block_compression = animation.use_block_compression;
		var old_default_duration = animation.default_duration;
		var old_cube_sides = animation.cube_side;

		animation.use_frames_subtraction = new_frame_subtraction;
		animation.use_block_compression = new_block_compression;
		animation.default_duration = new_default_duration;

		if (new_cube_sides != old_cube_sides) {
			if (animation.frames.length != 0 && !confirm("You are resetting the cube size. This will delete the current animation. Are you sure?")) {
				return;
			}

			clearAnimation();
			animation.frames = [];
			animation.cube_side = new_cube_sides;
			constructPage(animation);
		} else {
			// Just update
			updateUI(animation.cube_side);
		}

		$("#settings_modal").modal("hide");
	});



	$("#download_animation").click(function () {
		$("#download_modal_data").val(JSON.stringify(animation));
		$("#download_modal").modal("show");
	});
	$("#load_animation").click(function () {
		$("#load_modal").modal("show");
		$("#load_modal_error").text("");
	});
	$("#load_modal_save").click(function () {
		try {
			animation = JSON.parse($("#load_modal_data").val());
			frame_cursor = 0;

			saveAnimation();
			constructPage(animation);
			$("#load_modal").modal("hide");
		} catch (err) {
			$("#load_modal_error").text(err.message);
		}
	});
	$("#clear_animation").click(function () {
		clearAnimation();
		animation.frames = [];
		frame_cursor = 0;
		updateUI(animation.cube_side);
	});
});

function constructPage(animation) {
	// Generate the grid
	$("#grid").html(generateGrid(animation.cube_side));
	frame_cursor = 0;
	updateUI(animation.cube_side);
}

// Updates the UI based on current data
function updateUI(sides) {
	loadFrame(sides);

	if (isNewFrame()) {
		// The cursor is over the new frame position
		$("#frames_counter").text("Current frame: new / " + (animation.frames.length - 1) + "");
	} else {
		$("#frames_counter").text("Current frame: " + (frame_cursor) + " / " + (animation.frames.length - 1) + "");
	}

	$("#frame_range").attr("min", 0);
	$("#frame_range").attr("max", animation.frames.length - 1 + 1);	// We actually add a new frame position here to the range. This represents the new frame position
	$("#frame_range").val(frame_cursor);
}

// Modifies the ui according to current frame and animation
function loadFrame(sides) {
	if (frame_cursor - animation.frames.length > 0) {
		// This means the cursor is 2 frames too many after the end of the animation
		// This should not happen
		alert("Error. See logs");
		throw "Frame cursor points beyond event the new frame position";
	}
	if (isNewFrame()) {
		// The cursor is over the new frame position
		// Clear ui
		$("#frame_duration").val(animation.default_duration);
		$("#orientation_bottom_up").prop("checked", true);
		$(".grid_cell").removeClass("active");
		return;
	}
	var frame_data = animation.frames[frame_cursor];

	$("#frame_duration").val(frame_data.duration);
	$("input[name=orientationradio]:eq(" + frame_data.orientation + ")").prop("checked", true);
	loadGridState(sides, frame_data.state);
}

function loadGridState(sides, grid_state) {
	for (var plane_id = 0; plane_id < sides; plane_id++) {
		var plane = $("div").find("[data-plane=" + plane_id + "]");
		loadPlaneState(sides, plane, grid_state[plane_id]);
	}
}
function loadPlaneState(sides, plane, plane_state) {
	for (var row_id = 0; row_id < sides; row_id++) {
		for (var col_id = 0; col_id < sides; col_id++) {
			var grid_cell = $(plane).find("[data-row=" + row_id + "]").find("[data-col=" + col_id + "]");

			// Reset first the cell
			$(grid_cell).removeClass("active");

			if (plane_state[row_id][col_id]) {
				$(grid_cell).addClass("active");
			}
		}
	}
}

function saveAnimation() {
	localStorage.setItem("animation", JSON.stringify(animation));
}
function loadAnimation() {
	if (localStorage.getItem("animation") != null) {
		animation = JSON.parse(localStorage.getItem("animation"));
	}
}
function clearAnimation() {
	localStorage.clear();
}

function isNewFrame(frame_id = null) {
	if (frame_id == null) frame_id = frame_cursor;
	if (frame_id - animation.frames.length >= 0) {
		return true;
	}
	return false;
}