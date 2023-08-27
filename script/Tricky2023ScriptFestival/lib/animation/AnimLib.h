

let ANIM_IDLE = "Idle";
let ANIM_MOVE = "Move";
let ANIM_MOVE_RIGHT = "MoveRight";
let ANIM_MOVE_LEFT = "MoveLeft";
let ANIM_MOVEEND = "MoveToIdle";
let ANIM_MOVEEND_RIGHT = "MoveRightToIdle";
let ANIM_MOVEEND_LEFT = "MoveLeftToIdle";
let ANIM_ATTACK = "Attack";
let ANIM_ATTACKEND = "AttackToIdle";
let fl = false;

let AuraCast = false;
let AuraAnimMod = 0;
let AuraCount = 0;
let AuraGovern = 0;
let AuraMoveLeft = false;
let AuraMoveRight = false;
let AURACOLOUR = 0;

function ObjAnim_AddFrameA1(objID, type, left, top, right, bot){
	let aarr = Obj_GetValueD(objID, "anim_"~type, []);
	Obj_SetValue(objID, "anim_"~type, aarr~[[left, top, right, bot, 0, 0]]);
}

function ObjAnim_AddFrameA2(objID, type, left, top, right, bot, xoff, yoff){
	let aarr = Obj_GetValueD(objID, "anim_"~type, []);
	Obj_SetValue(objID, "anim_"~type, aarr~[[left, top, right, bot, xoff, yoff]]);
}

function ObjAnim_SetLoopPointA1(objID, type, frame){
	Obj_SetValue(objID, "anim_"~type~"_loop", frame);
}

function ObjAnim_SetSpeedA1(objID, type, speed){
	Obj_SetValue(objID, "anim_"~type~"_speed", speed);
}

function ObjAnim_FlipMoveImage(objID, cond){
	fl = cond;
}

function ObjAnim_ForceStop(objID){
	Obj_SetValue(objID, "anim_forcestop", true);
}

function ObjAnim_SetAttackA1(objID, status){
	if(Obj_IsDeleted(objID)){return false;}
	DidUserSetFrames(objID, ANIM_ATTACK, "Attack");
	DidUserSetFrames(objID, ANIM_ATTACKEND, "Attack");
	if(!Obj_IsValueExists(objID, "anim_Attack_loop")){
		RaiseError("Did not specify loop value for attack animation");
	}
	Obj_SetValue(objID, "anim_playattack", status);
}

function ObjAnim_ReplaceFrameA2(objID, type, animindex, left, top, right, bot, xoff, yoff){
	if(Obj_IsDeleted(objID)){return;}
 let aarr = Obj_GetValueD(objID, "anim_"~type, []);
 aarr[animindex] = [left, top, right, bot, xoff, yoff];
 Obj_SetValue(objID, "anim_"~type, aarr);
}
task ObjAnim_SetAttackA2(objID, len){
	if(Obj_IsDeleted(objID)){return;}
	DidUserSetFrames(objID, ANIM_ATTACK, "Attack");
	DidUserSetFrames(objID, ANIM_ATTACKEND, "Attack To Idle (ATTACKEND)");
	if(!Obj_IsValueExists(objID, "anim_Attack_loop")){
		RaiseError("Did not specify loop value for attack animation");
	}
	Obj_SetValue(objID, "anim_playattack", true);
	loop(len){yield;}
	Obj_SetValue(objID, "anim_playattack", false);
}

function ObjAnim_IsAttackPlaying(objID){
	return Obj_GetValueD(objID, "anim_isattackplaying", false);
}

task ObjAnim_StartAnimationA1(objID, bob, bobspeed){
	DidUserSetFrames(objID, ANIM_IDLE, "Idle");
	DidUserSetFrames(objID, ANIM_MOVE, "Move");
	DidUserSetFrames(objID, ANIM_MOVEEND, "Move To Idle (MOVEEND)");
	DidUserSetValidLoop(objID, ANIM_MOVE, "");
	
	let anim_count = 0;
	let anim_type = 0;
	let anim_rend = 0;
	
	let idle = Obj_GetValueD(objID, "anim_"~ANIM_IDLE~"_speed", 3);
	let move = Obj_GetValueD(objID, "anim_"~ANIM_MOVE~"_speed", 4);
	let attack = Obj_GetValueD(objID, "anim_"~ANIM_ATTACK~"_speed", 6);
	
	let AnimAttack = false;
	
	let x = 0;
	let y = 0;
	let fr = [];
	
	let m = 0;
	
	Obj_SetValue(objID, "anim_isattackplaying", false);
	
	while(!Obj_IsDeleted(objID)&&!Obj_GetValueD(objID, "anim_forcestop", false)){
		AnimAttack = Obj_GetValueD(objID, "anim_playattack", false);
		if(!AnimAttack){
			if(ObjMove_GetSpeed(objID) == 0){
				if(anim_type == 1){
					anim_count = 0;
					anim_type = 2;
				}
			}
			if(ObjMove_GetSpeed(objID) > 0){
				if(anim_type == 0){
					anim_count = 0;
					anim_type = 1;
				}
			}
			if(anim_type == 3){
				anim_type=4;
				anim_count = 0;
			}
		}else{
			Obj_SetValue(objID, "anim_isattackplaying", true);
			if(anim_type != 3){
				anim_type=3;
				anim_count = 0;
			}
		}
		
		if(CheckDirectionAL1(objID, "right")){ObjRender_SetScaleXYZ(objID, 1-2*fl, 1, 1);}
		if(CheckDirectionAL1(objID, "left")){ObjRender_SetScaleXYZ(objID, -1+2*fl, 1, 1);}
		
			
		if(anim_type==1){ //Moving
			anim_count+=1/move;
			if(floor(anim_count)>length(Obj_GetValue(objID, "anim_"~ANIM_MOVE))-1){anim_count = Obj_GetValue(objID, "anim_"~ANIM_MOVE~"_loop");}
			anim_rend = floor(anim_count);
			if(Obj_IsDeleted(objID)){break;}
			fr = Obj_GetValue(objID, "anim_"~ANIM_MOVE)[anim_rend];
		}
		if(anim_type==2){ //Moving to idle
			anim_count+=1/move;
			if(floor(anim_count)>length(Obj_GetValue(objID, "anim_"~ANIM_MOVEEND))-1){anim_count = 0;anim_type=0;}
			anim_rend = floor(anim_count);
			if(Obj_IsDeleted(objID)){break;}
			fr = Obj_GetValue(objID, "anim_"~ANIM_MOVEEND)[anim_rend];
		}
		if(anim_type==3){ //Attack
			anim_count+=1/attack;
			if(floor(anim_count)>length(Obj_GetValue(objID, "anim_"~ANIM_ATTACK))-1){anim_count = Obj_GetValue(objID, "anim_"~ANIM_ATTACK~"_loop");}
			anim_rend = floor(anim_count);
			if(Obj_IsDeleted(objID)){break;}
			fr = Obj_GetValue(objID, "anim_"~ANIM_ATTACK)[anim_rend];
		}
		if(anim_type==4){ //Attack to idle
			anim_count+=1/attack;
			if(floor(anim_count)>length(Obj_GetValue(objID, "anim_"~ANIM_ATTACKEND))-1){anim_count = 0;anim_type=0;Obj_SetValue(objID, "anim_isattackplaying", false);}
			anim_rend = floor(anim_count);
			if(Obj_IsDeleted(objID)){break;}
			fr = Obj_GetValue(objID, "anim_"~ANIM_ATTACKEND)[anim_rend];
		}
		if(anim_type==0){ //Idle
			anim_count+=1/idle;
			if(floor(anim_count)>length(Obj_GetValue(objID, "anim_"~ANIM_IDLE))-1){anim_count = Obj_GetValueD(objID, "anim_"~ANIM_IDLE~"_loop", 0);}
			anim_rend = floor(anim_count);
			if(Obj_IsDeleted(objID)){break;}
			fr = Obj_GetValue(objID, "anim_"~ANIM_IDLE)[anim_rend];
		}
		x = fr[2]-fr[0];
		y = fr[3]-fr[1];
		
		SetCommonData("rects", fr);
		m+=bobspeed;
		ObjSprite2D_SetSourceRect(objID, fr[0], fr[1], fr[2], fr[3]);
		ObjSprite2D_SetDestRect(objID, -x/2+fr[4]/2, -y/2+(bob*sin(m))+fr[5]/2, x/2+fr[4]/2, y/2+(bob*sin(m))+fr[5]/2);
		yield;
	}
	function EmergencyCheck(container){
		
		
	}	
}

task ObjAnim_StartAnimationA2(objID, bob, bobspeed){
	DidUserSetFrames(objID, ANIM_IDLE, "Idle");
	DidUserSetFrames(objID, ANIM_MOVE_RIGHT, "Move (right)");
	DidUserSetFrames(objID, ANIM_MOVE_LEFT, "Move (left)");
	DidUserSetFrames(objID, ANIM_MOVEEND_RIGHT, "Move To Idle (MOVEEND_RIGHT)");
	DidUserSetFrames(objID, ANIM_MOVEEND_LEFT, "Move To Idle (MOVEEND_LEFT)");
	DidUserSetValidLoop(objID, ANIM_MOVE_RIGHT, " (right)");
	DidUserSetValidLoop(objID, ANIM_MOVE_LEFT, " (left)");
	
	let anim_count = 0;
	let anim_type = 0;
	let anim_rend = 0;
	
	let idle = Obj_GetValueD(objID, "anim_"~ANIM_IDLE~"_speed", 3);
	let mover = Obj_GetValueD(objID, "anim_"~ANIM_MOVE_RIGHT~"_speed", 4);
	let movel = Obj_GetValueD(objID, "anim_"~ANIM_MOVE_LEFT~"_speed", 4);
	let attack = Obj_GetValueD(objID, "anim_"~ANIM_ATTACK~"_speed", 6);
	
	let AnimAttack = false;
	
	let x = 0;
	let y = 0;
	let fr = [];
	
	let m = 0;
	let LastDirection;
			
	Obj_SetValue(objID, "anim_isattackplaying", false);
	
	while(!Obj_IsDeleted(objID)&&!Obj_GetValueD(objID, "anim_forcestop", false)){
		AnimAttack = Obj_GetValueD(objID, "anim_playattack", false);
		if(!AnimAttack){
			if(ObjMove_GetSpeed(objID) == 0){
				if(anim_type == 1){
					anim_count = 0;
					anim_type = 2;
				}
			}
			if(ObjMove_GetSpeed(objID) > 0){
				if(anim_type == 0){
					anim_count = 0;
					anim_type = 1;
				}
			}
			if(anim_type == 3){
				anim_type=4;
				anim_count = 0;
			}
		}else{
			Obj_SetValue(objID, "anim_isattackplaying", true);
			if(anim_type != 3){
				anim_type=3;
				anim_count = 0;
			}
		}
		
		if(CheckDirectionAL1(objID, "right")){LastDirection = "Right";}
		if(CheckDirectionAL1(objID, "left")){LastDirection = "Left";}
		
			
		if(anim_type==1){ //Moving
			if(LastDirection == "Right"){
				anim_count+=1/mover;
				if(floor(anim_count)>length(Obj_GetValue(objID, "anim_"~ANIM_MOVE_RIGHT))-1){anim_count = Obj_GetValue(objID, "anim_"~ANIM_MOVE_RIGHT~"_loop");}
				anim_rend = floor(anim_count);
				if(Obj_IsDeleted(objID)){break;}
				fr = Obj_GetValue(objID, "anim_"~ANIM_MOVE_RIGHT)[anim_rend];
			}
			else if(LastDirection == "Left"){
				anim_count+=1/movel;
				if(floor(anim_count)>length(Obj_GetValue(objID, "anim_"~ANIM_MOVE_LEFT))-1){anim_count = Obj_GetValue(objID, "anim_"~ANIM_MOVE_LEFT~"_loop");}
				anim_rend = floor(anim_count);
				if(Obj_IsDeleted(objID)){break;}
				fr = Obj_GetValue(objID, "anim_"~ANIM_MOVE_LEFT)[anim_rend];
			}
			else{RaiseError("An error occurred when animating. If you cannot resolve the issue, please send the affected script to gtbot so he may fix it.");}
		}
		if(anim_type==2){ //Moving to idle
			if(LastDirection == "Right"){
				anim_count+=1/mover;
				if(floor(anim_count)>length(Obj_GetValue(objID, "anim_"~ANIM_MOVEEND_RIGHT))-1){anim_count = 0;anim_type=0;}
				anim_rend = floor(anim_count);
				if(Obj_IsDeleted(objID)){break;}
				fr = Obj_GetValue(objID, "anim_"~ANIM_MOVEEND_RIGHT)[anim_rend];
			}
			else if(LastDirection == "Left"){
				anim_count+=1/movel;
				if(floor(anim_count)>length(Obj_GetValue(objID, "anim_"~ANIM_MOVEEND_LEFT))-1){anim_count = 0;anim_type=0;}
				anim_rend = floor(anim_count);
				if(Obj_IsDeleted(objID)){break;}
				fr = Obj_GetValue(objID, "anim_"~ANIM_MOVEEND_LEFT)[anim_rend];
			}
			else{RaiseError("An error occurred when animating. If you cannot resolve the issue, please send the affected script to gtbot so he may fix it.");}
		}
		if(anim_type==3){ //Attack
			anim_count+=1/attack;
			if(floor(anim_count)>length(Obj_GetValue(objID, "anim_"~ANIM_ATTACK))-1){anim_count = Obj_GetValue(objID, "anim_"~ANIM_ATTACK~"_loop");}
			anim_rend = floor(anim_count);
			if(Obj_IsDeleted(objID)){break;}
			fr = Obj_GetValue(objID, "anim_"~ANIM_ATTACK)[anim_rend];
		}
		if(anim_type==4){ //Attack to idle
			anim_count+=1/attack;
			if(floor(anim_count)>length(Obj_GetValue(objID, "anim_"~ANIM_ATTACKEND))-1){anim_count = 0;anim_type=0;Obj_SetValue(objID, "anim_isattackplaying", false);}
			anim_rend = floor(anim_count);
			if(Obj_IsDeleted(objID)){break;}
			fr = Obj_GetValue(objID, "anim_"~ANIM_ATTACKEND)[anim_rend];
		}
		if(anim_type==0){ //Idle
			anim_count+=1/idle;
			if(floor(anim_count)>length(Obj_GetValue(objID, "anim_"~ANIM_IDLE))-1){anim_count = Obj_GetValueD(objID, "anim_"~ANIM_IDLE~"_loop", 0);}
			anim_rend = floor(anim_count);
			if(Obj_IsDeleted(objID)){break;}
			fr = Obj_GetValue(objID, "anim_"~ANIM_IDLE)[anim_rend];
		}
		x = fr[2]-fr[0];
		y = fr[3]-fr[1];
		
		SetCommonData("rects", fr);
		
		m+=bobspeed;
		ObjSprite2D_SetSourceRect(objID, fr[0], fr[1], fr[2], fr[3]);
		ObjSprite2D_SetDestRect(objID, -x/2+fr[4]/2, -y/2+(bob*sin(m))+fr[5]/2, x/2+fr[4]/2, y/2+(bob*sin(m))+fr[5]/2);
		yield;
	}
	function EmergencyCheck(container){

	}	
}



function GetAnimationFrames(objID, type){
	return Obj_GetValue(objID, "anim_"~type);
}

function DidUserSetFrames(objID, type, str){
	if(!(length(Obj_GetValueD(objID, "anim_"~type, []))>0)){
		RaiseError("Did not specify frames for "~str~" Animation");
	}
}

function DidUserSetValidLoop(objID, type, str){
	if(!Obj_IsValueExists(objID, "anim_"~type)){
		RaiseError("Did not specify loop value for movement ("~str~") animation");
	}
	let loopvar = Obj_GetValue(objID, "anim_"~type~"_loop");
	let loopnum = length(Obj_GetValue(objID, "anim_"~type))-1;
	if(loopvar>loopnum){
		RaiseError("Specified loop value of "~IntToString(loopvar)~" for movement"~str~" exceeds total number of frame indices: "~IntToString(loopnum));
	}
	else if(loopvar<0){
		RaiseError("Specified loop value of "~IntToString(loopvar)~" for movement"~str~" is invalid: cannot be less than 0");
	}
}

function CheckDirectionAL1(checkid, direc){
	let direction = false;
	let enemyang = absolute(ObjMove_GetAngle(checkid));
	let enemyspd = absolute(ObjMove_GetSpeed(checkid));
	
	
	while(enemyang>360){
		enemyang = enemyang-360;
	}

	if(enemyspd > 0){
		if(direc=="right"){
			if(enemyang > 270 || enemyang-270 < 90){
				direction=true;
			}
		}
		if(direc=="left"){
			if(enemyang < 270 && enemyang > 90){
				direction=true;
			}
		}
	}
	if(direc=="none"){
		if(enemyspd == 0){
			direction=true;
		}
	}
	
	return direction;
}

function Animate(anim_count){
	return max(0, floor(anim_count));
}


task AuraGraphic(tobj, graphic, RC, GC, BC){
	let ALPHA= 250;
	let MOD = 0;
	let rate = 2/90 + rand(0.0125, -0.0125);
	let frames = GetCommonData("rects", [0, 0, 128, 128]);
	let obj = ObjPrim_Create(OBJ_SPRITE_2D);
	ObjPrim_SetTexture(obj, graphic);
	ObjSprite2D_SetSourceRect(obj, frames[0], frames[1], frames[2], frames[3]);
	ObjRender_SetScaleXYZ(obj, 1.1, 1.1, NULL);
	ObjSprite2D_SetDestCenter(obj);
	ObjRender_SetColor(obj, RC, GC, BC);
	ObjRender_SetBlendType(obj, BLEND_ADD_ARGB);
	ObjRender_SetAlpha(obj, ALPHA);
	Obj_SetRenderPriorityI(obj, 27);
	
	ObjRender_SetPosition(obj, ObjMove_GetX(tobj), ObjMove_GetY(tobj), 0);
	
	AuraAnim(obj, 12);
	
	

	
	while(!Obj_IsDeleted(tobj)){
		frames = GetCommonData("rects", [0, 0, 128, 128]);
		ObjRender_SetAlpha(obj, ALPHA);
		ObjRender_SetScaleXYZ(obj, 1.1+MOD, 1.1+MOD, NULL);
		//ObjSprite2D_SetSourceRect(obj, frames[0], frames[1], frames[2], frames[3]);
		MOD+=rate;
		
		if(ALPHA<=0){
			Obj_Delete(obj);
			}
			
		ALPHA-=6;
		
		loop(2){ yield; }
		}
		
	Obj_Delete(obj);
}

task AuraGraphic2_A(tobj, graphic, RC, GC, BC){
	let ALPHA= 250;
	let MOD = 0;
	let rate = 2/90 + rand(0.0125, -0.0125);
	let frames = GetCommonData("rects", [0, 0, 128, 128]);
	let obj = ObjPrim_Create(OBJ_SPRITE_2D);
	ObjPrim_SetTexture(obj, graphic);
	ObjSprite2D_SetSourceRect(obj, frames[0], frames[1], frames[2], frames[3]);
	ObjRender_SetScaleXYZ(obj, 1.5, 1.5, NULL);
	ObjSprite2D_SetDestCenter(obj);
	ObjRender_SetColor(obj, RC, GC, BC);
	ObjRender_SetBlendType(obj, BLEND_ADD_ARGB);
	ObjRender_SetAlpha(obj, ALPHA);
	Obj_SetRenderPriorityI(obj, 26);
	
	ObjRender_SetPosition(obj, ObjMove_GetX(tobj), ObjMove_GetY(tobj), 0);
	
	AuraAnim(obj, 12);
	
	

	
	while(!Obj_IsDeleted(tobj)){
		frames = GetCommonData("rects", [0, 0, 128, 128]);
		ObjRender_SetAlpha(obj, ALPHA);
		ObjRender_SetScaleXYZ(obj, 0.7+(MOD/1.5), 0.7+(MOD/1.5), NULL);
		//ObjSprite2D_SetSourceRect(obj, frames[0], frames[1], frames[2], frames[3]);
		MOD+=rate;
		
		if(ALPHA<=0){
			Obj_Delete(obj);
			}
			
		ALPHA-=25; //6
		
		loop(2){ yield; }
		}
		
	Obj_Delete(obj);
}

task AuraGraphic2_B(tobj, graphic, RC, GC, BC){
	let ALPHA= 250;
	let MOD = 0;
	let rate = 2/90 + rand(0.0125, -0.0125);
	let frames = GetCommonData("rects", [0, 0, 128, 128]);
	let obj = ObjPrim_Create(OBJ_SPRITE_2D);
	ObjPrim_SetTexture(obj, graphic);
	ObjSprite2D_SetSourceRect(obj, frames[0], frames[1], frames[2], frames[3]);
	ObjRender_SetScaleXYZ(obj, 1.1, 1.1, NULL);
	ObjSprite2D_SetDestCenter(obj);
	ObjRender_SetColor(obj, RC, GC, BC);
	ObjRender_SetBlendType(obj, BLEND_SUBTRACT);
	ObjRender_SetAlpha(obj, ALPHA);
	Obj_SetRenderPriorityI(obj, 27);
	
	ObjRender_SetPosition(obj, ObjMove_GetX(tobj), ObjMove_GetY(tobj), 0);
	
	AuraAnim(obj, 12);
	
	

	
	while(!Obj_IsDeleted(tobj)){
		frames = GetCommonData("rects", [0, 0, 128, 128]);
		ObjRender_SetAlpha(obj, ALPHA);
		ObjRender_SetScaleXYZ(obj, 0.7+(MOD/1.5), 0.7+(MOD/1.5), NULL);
		//ObjSprite2D_SetSourceRect(obj, frames[0], frames[1], frames[2], frames[3]);
		MOD+=rate;
		
		if(ALPHA<=0){
			Obj_Delete(obj);
			}
			
		ALPHA-=25; //6
		
		loop(2){ yield; }
		}
		
	Obj_Delete(obj);
}


task AuraGraphic2(tobj, graphic, RC, GC, BC){
	let ALPHA= 250;
	let MOD = 0;
	let rate = 2/90 + rand(0.0125, -0.0125);
	let frames = GetCommonData("rects", [0, 0, 128, 128]);
	let obj = ObjPrim_Create(OBJ_SPRITE_2D);
	ObjPrim_SetTexture(obj, graphic);
	ObjSprite2D_SetSourceRect(obj, frames[0], frames[1], frames[2], frames[3]);
	ObjRender_SetScaleXYZ(obj, 0.7, 0.7, NULL);
	ObjSprite2D_SetDestCenter(obj);
	ObjRender_SetColor(obj, RC, GC, BC);
	ObjRender_SetBlendType(obj, BLEND_ADD_ARGB);
	ObjRender_SetAlpha(obj, ALPHA);
	Obj_SetRenderPriorityI(obj, 27);
	
	ObjRender_SetPosition(obj, ObjMove_GetX(tobj), ObjMove_GetY(tobj), 0);
	
	AuraAnim(obj, 12);
	
	

	
	while(!Obj_IsDeleted(tobj)){
		frames = GetCommonData("rects", [0, 0, 128, 128]);
		ObjRender_SetAlpha(obj, ALPHA);
		ObjRender_SetScaleXYZ(obj, 0.7+(MOD/1.5), 0.7+(MOD/1.5), NULL);
		//ObjSprite2D_SetSourceRect(obj, frames[0], frames[1], frames[2], frames[3]);
		MOD+=rate;
		
		if(ALPHA<=0){
			Obj_Delete(obj);
			}
			
		ALPHA-=6;
		
		loop(2){ yield; }
		}
		
	Obj_Delete(obj);
}


task Aura(tobj, graphic, RC, GC, BC){
	
	let frame =0;
	
	while(!Obj_IsDeleted(tobj)){
		frame++;
		if(GetCommonData("turnoffaura", false)==true){
			break;
		
		}
		if(frame==5){
			AuraGraphic(tobj, graphic, RC, GC, BC);
			frame=0;
			}
			
		RC-=3*sin(AURACOLOUR);
		GC-=3*sin(AURACOLOUR);
		BC-=3*sin(AURACOLOUR);
		AURACOLOUR+=2;
			
		yield;
		}
		
	}

task Aura2(tobj, graphic, RC, GC, BC){
	
	let frame =0;
	
	while(!Obj_IsDeleted(tobj)){
		frame++;
		if(GetCommonData("turnoffaura", false)==true){
			break;
		
		}
		if(frame==5){
			AuraGraphic2(tobj, graphic, RC, GC, BC);
			frame=0;
			}
			
		RC-=3*sin(AURACOLOUR);
		GC-=3*sin(AURACOLOUR);
		BC-=3*sin(AURACOLOUR);
		AURACOLOUR+=2;
			
		yield;
		}
		
	}

task Aura_A(tobj, graphic){
	
	let frame =0;
	let RC = 255;
	let GC = 255;
	let BC = 200;
	
	while(!Obj_IsDeleted(tobj)){
		frame++;
		if(GetCommonData("turnoffaura", false)==true){
			break;
		
		}
		if(frame==5){
			AuraGraphic2_A(tobj, graphic, 255, 255, 200);
			AuraGraphic2_B(tobj, graphic, 0, 0, 55);
			frame=0;
			}
			
		RC-=3*sin(AURACOLOUR);
		GC-=3*sin(AURACOLOUR);
		BC-=3*sin(AURACOLOUR);
		AURACOLOUR+=2;
			
		yield;
		}
		
}
task Aura_D(tobj, graphic){
	
	let frame =0;
	let RC = 255;
	let GC = 0;
	let BC = 0;
	
	while(!Obj_IsDeleted(tobj)){
		frame++;
		if(GetCommonData("turnoffaura", false)==true){
			break;
		
		}
		if(frame==5){
			AuraGraphic2_A(tobj, graphic, 255, 0, 0);
			AuraGraphic2_B(tobj, graphic, 0, 255, 255);
			frame=0;
			}
			
		RC-=3*sin(AURACOLOUR);
		GC-=3*sin(AURACOLOUR);
		BC-=3*sin(AURACOLOUR);
		AURACOLOUR+=2;
			
		yield;
		}
		
}


task AuraAnim(tobj, Yield){
	ObjSprite2D_SetDestCenter(tobj);
	
	}
