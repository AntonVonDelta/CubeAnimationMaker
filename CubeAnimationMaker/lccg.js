function getLCCGFrames() {
    // Standardize text
    var text = $("#load_lccg_modal_data").val().replace(/\r\n/g, "\n");

    var lccg_frames = loadLCCG(text);
    var ui_orientation = parseInt($("input[name=orientationradio]:checked").val());

    // Set ui_orientation so that the user doesn't have to change it manually after loading
    return lccg_frames.map(x => ({ duration: x.duration, orientation: ui_orientation, state: x.state }));
}


// Loads LCCG data and returns grid state array.
// Throws error if text not formatted properly
function loadLCCG(text) {
    var result = [];
    var format_check_result = checkFormat(text);
    if (format_check_result != null) {
        throw { message: format_check_result };
    }

    var frame_regex = /(?:B[01]{4}\s*,?\s*){16}\s*(\d+)\s*,?\s*?/g;
    var bit_regex = /[01]/g;;
    while (match_frame = frame_regex.exec(text)) {
        var duration = parseInt(match_frame[1]);
        var bit_array = match_frame[0].match(bit_regex);
        var frame_data = [];

        for (var plane = 0; plane < 4; plane++) {
            var rows = [];
            for (var row = 0; row < 4; row++) {
                var cols = [];
                for (var col = 0; col < 4; col++) {
                    cols.push(getLCCGCellState(bit_array, plane, row, col) == "0" ? 0 : 1);
                }
                rows.push(cols);
            }
            frame_data.push(rows);
        }

        result.push({ duration: duration, state: frame_data });
    }

    return result;
}

// Returns error message or null if format checks out
function checkFormat(text) {
    // Matches either lccg syntax or comments
    var lccg_regex = /(?:(?:\s*B[01]{4}\s*,?\s*){16}\s*\d+\s*,?\s*?)|(?:\s*\/\/\s*.*\n?)/g;
    var last_index = 0;

    while (match = lccg_regex.exec(text)) {
        if (match.index != last_index) {
            return "Unknown syntax between positions " + last_index + " and " + match.index + ". " + getLCCGCharPosition(text, last_index, text.length);
        }

        last_index = match.index + match[0].length;
    }
    if (last_index != text.length) {
        return "Unknown syntax between positions " + last_index + " and " + (text.length - 1) + ". " + getLCCGCharPosition(text, last_index, text.length);
    }
    return null;
}

// Returns text which describes the position of the index in the text
function getLCCGCharPosition(text, start_index, end_index) {
    var before_index_substr = text.substr(0, start_index).replace(/\r\n/g, "\n");
    var line = text.substr(start_index, end_index - start_index);
    var count_newlines = (before_index_substr.match(/\n/g) || []).length + 1;
    var count_columns = 1;

    // Find the previous new line in order to calculate chars from start of line
    for (var i = start_index - 1; i >= 0; i--) {
        if (before_index_substr[i] == '\n') break;
        count_columns++;
    }

    return "Line #" + count_newlines + ", col #" + count_columns + ", text:'" + line + "'";
}

// Returns cell value correspinding to LCCG grid (as seen in the application)
// We assume that plane 0 is the first plane and that row 0 and col 0 start at the upper left corner of each plane
// We make this assumption because this is how this editor arranges the cells
function getLCCGCellState(arr, plane, row, col) {
    return arr[(3 - row) * 4 * 4 + (3 - plane) * 4 + col];
}