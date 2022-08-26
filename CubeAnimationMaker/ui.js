function getFrame(sides) {
	var duration = $("#frame_duration").val();
	var orientation = parseInt($("input[name=orientationradio]:checked").val());
	if (duration == "") duration = "0";

	return { duration: parseInt(duration), orientation: orientation, state: constructGridState(sides) };
}

// Returns 2D array with all the states of the pressed buttons
function constructGridState(sides) {
	var result = [];

	for (var plane_id = 0; plane_id < sides; plane_id++) {
		var plane = $("div").find("[data-plane=" + plane_id + "]");
		result.push(constructPlaneState(sides, plane));
	}
	return result;
}
function constructPlaneState(sides, plane) {
	var result = [];

	for (var row_id = 0; row_id < sides; row_id++) {
		var row = [];
		for (var col_id = 0; col_id < sides; col_id++) {
			var grid_cell = $(plane).find("[data-row=" + row_id + "]").find("[data-col=" + col_id + "]");
			row.push($(grid_cell).hasClass("active") ? 1 : 0);
		}
		result.push(row);
	}
	return result;
}

// Updates the grid with the given state
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



// Generates html code for the grid based on given size
function generateGrid(sides) {
	var result = "";
	for (var i = 0; i < sides; i++) {
		result += generatePlane(sides, i);
	}
	return result;
}
function generatePlane(sides, plane) {
	var template = $("#template_plane").html();
	var rows = "";
	var result = template.replace("%s", plane);

	for (var i = 0; i < sides; i++) {
		rows += generateRow(sides, plane, i);
	}
	result = result.replace("%s", rows);
	return result;
}
function generateRow(sides, plane, row) {
	var template = $("#template_plane_row").html();
	var row_cells = "";
	var result = template.replace("%s", row);

	for (var i = 0; i < sides; i++) {
		row_cells += generateCell(sides, plane, row, i);
	}
	result = result.replace("%s", row_cells);
	return result;

}
function generateCell(sides, plane, row, col) {
	var template = $("#template_plane_cell").html();
	template = template.replace("%s", row);
	template = template.replace("%s", col);
	template = template.replace("%s", col);
	return template;
}


