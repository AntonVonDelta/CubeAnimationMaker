const cube_side=4;
const default_duration=10;
var frame_cursor=0;	// Slider cursor: 0 means begining of animation
var animation=[];

	
$(document).ready(function(){
/* 	Algo ideea:
		Get the difference between two frames. Many animations share similar consecutive frames.
		This will reduce all the information in one scene to almost all zeroes.
		This can then be compressed per frame.

		
	Byte format C++ (4bytes long):
		Count = 4 bits: 	how many times the next bit repeats
		Value =	1 bit: 		the value of the leds
		
		Special case if Count==0:
			no benefit gained from compression (possible if the displayed pattern is like Chess)
			read the data uncompressed till end of frame.
*/
	
	// Generate the grid
	$("#grid").html(generateGrid(cube_side));
	
	loadAnimation();
	updateUI(cube_side);	
	
	$("#insert_frame").click(function(){
		var frame=getFrame(cube_side);
		animation.splice(frame_cursor,0,frame);
		console.log(frame);
		
		frame_cursor+=1;
	});
	
	$("#frame_range").change(function(){
		var new_value=parseInt($(this).val());
		frame_cursor=new_value;
		updateUI(cube_side);
	});
	
	$(".grid_cell").click(function(e){
		$(this).button("toggle");
		console.log($(this).attr("data-col"));
		
	});
	$("#clear_all").click(function(){
		$(".grid_cell").removeClass("active");
		$(".grid_cell").removeAttr("aria-pressed");
	});
	$("#set_all").click(function(){
		$(".grid_cell").addClass("active");
		$(".grid_cell").attr("aria-pressed","true");
	});
	$("#btn1").click(function(){		
		$("#txt1").text(compress(animation).join());
	});
		
});

// Updates the UI based on current data
function updateUI(sides){
	loadCurrentFrame(sides);
	$("#frames_counter").text("Current frame: "+(frame_cursor+1)+" / "+animation.length+"");
	
	$("#frame_range").val(frame_cursor);
	$("#frame_range").attr("min",0);
	$("#frame_range").attr("max",animation.length-1);
}

// Modifies the ui according to current frame and animation
function loadCurrentFrame(sides){
	if(frame_cursor + 1 > animation.length){
		// No frames exist
		// Clear ui
		$("#frame_duration").val(default_duration);
		$(".grid_cell").removeClass("active");
		$(".grid_cell").removeAttr("aria-pressed");
		return;
	}
	var frame_data=animation[frame_cursor];
	
	$("#frame_duration").val(frame_data.duration);
	loadGridState(sides, frame_data.state);
}

function loadGridState(sides, grid_state){
	for(var plane_id=0;plane_id<sides;plane_id++){
		var plane=$("div").find("[data-plane="+plane_id+"]");
		loadPlaneState(sides, plane, grid_state[plane_id]);
	}
}
function loadPlaneState(sides, plane, plane_state){
	for(var row_id=0;row_id<sides;row_id++){
		for(var col_id=0;col_id<sides;col_id++){
			var grid_cell=$(plane).find("[data-row="+row_id+"]").find("[data-col="+col_id+"]");
			
			// Reset first the cell
			$(grid_cell).removeClass("active");
			$(grid_cell).removeAttr("aria-pressed");
			
			if(plane_state[row_id][col_id]){
				$(grid_cell).addClass("active");
				$(grid_cell).attr("aria-pressed","true");
			}
		}
	}
}

function saveAnimation(){
	localStorage.setItem("animation",JSON.stringify(animation));
}
function loadAnimation(){
	if(localStorage.getItem("animation")!=null){
		animation=JSON.parse(localStorage.getItem("animation"));
	}
}
function clearStorage(){
	localStorage.clear();
}