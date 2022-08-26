var frame_cursor = 0;			// Slider cursor: 0 means begining of animation
var animation = {
	use_frames_subtraction: true,
	use_block_compression: true,
	default_duration: 1,		// This is the unit-time. It will be multiplied in arduino code by the true standard frame duration
	default_orientation: 1,
	cube_side: 4,
	frames: []
};


$(document).ready(function () {
	var dragging = 0;
	var start_drag_element = null;

	loadAnimation();
	constructPage();

	$(".grid_cell").click(function (e) {
		$(this).button("toggle");
	});
	$("#clear_all").click(function () {
		$(".grid_cell").removeClass("active");
	});
	$("#set_all").click(function () {
		$(".grid_cell").addClass("active");
	});

	// Allow drag-selecting the grid cells
	$(document).on('mousedown', function () {
		dragging = 1;
	}).on('mouseup', function () {
		if (dragging == 2) {
			$(start_drag_element).addClass("active");
		}
		dragging = 0;
	});
	$(".grid_cell").on("mousedown", function (e) {
		start_drag_element = this;
	});
	$(".grid_cell").on("mouseenter", function (e) {
		if (!dragging) return;
		dragging = 2;
		$(this).addClass("active");
	});



	$("#insert_frame").click(function () {
		var frame = getFrame(animation.cube_side);

		animation.frames.splice(frame_cursor, 0, frame);
		frame_cursor += 1;
		updateUI();
	});
	$("#update_frame").click(function () {
		if (isNewFrame()) {
			alert("Frame #" + frame_id + " doesn't exist");
			return;
		}
		animation.frames.splice(frame_cursor, 1, getFrame(animation.cube_side));
		updateUI();
	});
	$("#refresh_frame").click(function () {
		updateUI();
	});
	$("#delete_frame").click(function () {
		if (isNewFrame()) {
			return;
		}
		animation.frames.splice(frame_cursor, 1);
		updateUI();
	});
	$("#seek_frame").click(function () {
		var frame_id = parseInt($("#seek_frame_id").val());
		if (isNewFrame(frame_id)) {
			alert("Frame #" + frame_id + " doesn't exist");
			return;
		}
		frame_cursor = frame_id;
		updateUI();
	});



	$("#frame_range").on("input change", function () {
		var new_value = parseInt($(this).val());
		frame_cursor = new_value;
		updateUI();
	});



	$("#generate_compress").click(function () {
		var compressed_binary = compressWithMetadata(animation).flat();
		var condensed_dwords = condenseBinary(compressed_binary);
		var formatted_list = condensed_dwords.map(x => x + "UL");
		var result = "const unsigned long PROGMEM animation[]={" + formatted_list.join() + "};";

		$("#output").val(result);
		$("#compression_ratio").text(compressionRatio(animation, compressed_binary) + "%");
		$("#compression_ratio_lccg").text(compressionRatioLCCG(animation, compressed_binary) + "%");
	});
	$("#settings").click(function () {
		$("#settings_frame_subtraction").prop("checked", animation.use_frames_subtraction);
		$("#settings_block_compression").prop("checked", animation.use_block_compression);
		$("#settings_default_duration").val(animation.default_duration);
		$("input[name=settingsorientationradio][value=" + animation.default_orientation + "]").prop("checked", true);
		$("#settings_cube_side").val(animation.cube_side);

		$("#settings_modal").modal("show");
	});
	$("#settings_modal_save").click(function () {
		var new_frame_subtraction = $("#settings_frame_subtraction").is(':checked');
		var new_block_compression = $("#settings_block_compression").is(':checked');
		var new_default_duration = parseInt($("#settings_default_duration").val());
		var new_default_orientation = parseInt(parseInt($("input[name=settingsorientationradio]:checked").val()));
		var new_cube_sides = parseInt($("#settings_cube_side").val());

		var old_frame_subtraction = animation.use_frames_subtraction;
		var old_block_compression = animation.use_block_compression;
		var old_default_duration = animation.default_duration;
		var old_default_duration = animation.default_orientation;
		var old_cube_sides = animation.cube_side;

		animation.use_frames_subtraction = new_frame_subtraction;
		animation.use_block_compression = new_block_compression;
		animation.default_duration = new_default_duration;
		animation.default_orientation = new_default_orientation;

		if (new_cube_sides != old_cube_sides) {
			if (animation.frames.length != 0 && !confirm("You are resetting the cube size. This will delete the current animation. Are you sure?")) {
				return;
			}

			clearAnimation();
			animation.frames = [];
			animation.cube_side = new_cube_sides;
			constructPage();
		} else {
			// Just update
			updateUI();
		}

		$("#settings_modal").modal("hide");
	});



	$("#save_animation").click(function () {
		saveAnimation();
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
			constructPage();
			$("#load_modal").modal("hide");
		} catch (err) {
			$("#load_modal_error").text(err.message);
		}
	});
	$("#load_modal_data").keyup(function () {
		// Clear error message if it was shown any
		$("#load_modal_error").text("");
	});

	$("#clear_animation").click(function () {
		clearAnimation();
		animation.frames = [];
		frame_cursor = 0;
		updateUI();
	});

	$("#load_lccg").click(function () {
		if (animation.cube_side != 4) {
			alert("LCCG can only be loaded for an 4x4 animation. Change cube size.");
			return;
		}
		$("#load_lccg_modal").modal("show");
		$("#load_lccg_modal_error").text("");
	});
	$("#load_lccg_modal_load").click(function () {
		try {
			var lccg_frames = getLCCGFrames();

			animation.frames.splice(frame_cursor, 0, ...lccg_frames);
			frame_cursor += lccg_frames.length;
			updateUI();

			$("#load_lccg_modal").modal("hide");
		} catch (err) {
			$("#load_lccg_modal_error").text(err.message);
		}
	});
	$("#load_lccg_modal_data").keyup(function () {
		// Clear error message if it was shown any
		$("#load_lccg_modal_error").text("");
	});
});

function constructPage() {
	// Generate the grid
	$("#grid").html(generateGrid(animation.cube_side));
	frame_cursor = 0;
	updateUI();
}

// Updates the UI based on current data
function updateUI() {
	loadFrame();

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
function loadFrame() {
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
		$("input[name=orientationradio][value=" + animation.default_orientation + "]").prop("checked", true);
		$(".grid_cell").removeClass("active");
		return;
	}
	var frame_data = animation.frames[frame_cursor];

	$("#frame_duration").val(frame_data.duration);
	$("input[name=orientationradio][value=" + frame_data.orientation + "]").prop("checked", true);
	loadGridState(animation.cube_side, frame_data.state);
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