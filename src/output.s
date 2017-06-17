	.data
	.align	2
	.globl	class_nameTab
	.globl	Main_protObj
	.globl	Int_protObj
	.globl	String_protObj
	.globl	bool_const0
	.globl	bool_const1
	.globl	_int_tag
	.globl	_bool_tag
	.globl	_string_tag
	.globl	_max_tag
	.globl	class_objTab
	.globl	class_parentTab
	.globl	class_attrTabTab
	.globl	Object_protObj
	.globl	Object_init
	.globl	Object_attrTab
	.globl	E606wxzslv_protObj
	.globl	E606wxzslv_init
	.globl	E606wxzslv_attrTab
	.globl	RJ4F9vxUtb_protObj
	.globl	RJ4F9vxUtb_init
	.globl	RJ4F9vxUtb_attrTab
	.globl	HNX5IGq5yQ_protObj
	.globl	HNX5IGq5yQ_init
	.globl	HNX5IGq5yQ_attrTab
	.globl	Main_protObj
	.globl	Main_init
	.globl	Main_attrTab
	.globl	UrfbZWZFyG_protObj
	.globl	UrfbZWZFyG_init
	.globl	UrfbZWZFyG_attrTab
	.globl	CRys1Atcrp_protObj
	.globl	CRys1Atcrp_init
	.globl	CRys1Atcrp_attrTab
	.globl	String_protObj
	.globl	String_init
	.globl	String_attrTab
	.globl	Bool_protObj
	.globl	Bool_init
	.globl	Bool_attrTab
	.globl	Int_protObj
	.globl	Int_init
	.globl	Int_attrTab
	.globl	IO_protObj
	.globl	IO_init
	.globl	IO_attrTab
_int_tag:
	.word	2
_bool_tag:
	.word	3
_string_tag:
	.word	4
	.globl	_MemMgr_INITIALIZER
_MemMgr_INITIALIZER:
	.word	_NoGC_Init
	.globl	_MemMgr_COLLECTOR
_MemMgr_COLLECTOR:
	.word	_NoGC_Collect
	.globl	_MemMgr_TEST
_MemMgr_TEST:
	.word	0
	.word	-1
str_const13:
	.word	4
	.word	5
	.word	String_dispTab
	.word	int_const0
	.byte	0	
	.align	2
	.word	-1
str_const12:
	.word	4
	.word	7
	.word	String_dispTab
	.word	int_const1
	.ascii	"UrfbZWZFyG"
	.byte	0	
	.align	2
	.word	-1
str_const11:
	.word	4
	.word	6
	.word	String_dispTab
	.word	int_const2
	.ascii	"Main"
	.byte	0	
	.align	2
	.word	-1
str_const10:
	.word	4
	.word	7
	.word	String_dispTab
	.word	int_const1
	.ascii	"CRys1Atcrp"
	.byte	0	
	.align	2
	.word	-1
str_const9:
	.word	4
	.word	7
	.word	String_dispTab
	.word	int_const1
	.ascii	"HNX5IGq5yQ"
	.byte	0	
	.align	2
	.word	-1
str_const8:
	.word	4
	.word	7
	.word	String_dispTab
	.word	int_const1
	.ascii	"RJ4F9vxUtb"
	.byte	0	
	.align	2
	.word	-1
str_const7:
	.word	4
	.word	7
	.word	String_dispTab
	.word	int_const1
	.ascii	"E606wxzslv"
	.byte	0	
	.align	2
	.word	-1
str_const6:
	.word	4
	.word	6
	.word	String_dispTab
	.word	int_const3
	.ascii	"String"
	.byte	0	
	.align	2
	.word	-1
str_const5:
	.word	4
	.word	6
	.word	String_dispTab
	.word	int_const2
	.ascii	"Bool"
	.byte	0	
	.align	2
	.word	-1
str_const4:
	.word	4
	.word	5
	.word	String_dispTab
	.word	int_const4
	.ascii	"Int"
	.byte	0	
	.align	2
	.word	-1
str_const3:
	.word	4
	.word	5
	.word	String_dispTab
	.word	int_const5
	.ascii	"IO"
	.byte	0	
	.align	2
	.word	-1
str_const2:
	.word	4
	.word	6
	.word	String_dispTab
	.word	int_const3
	.ascii	"Object"
	.byte	0	
	.align	2
	.word	-1
str_const1:
	.word	4
	.word	8
	.word	String_dispTab
	.word	int_const6
	.ascii	"<basic class>"
	.byte	0	
	.align	2
	.word	-1
str_const0:
	.word	4
	.word	7
	.word	String_dispTab
	.word	int_const7
	.ascii	"output.cl"
	.byte	0	
	.align	2
	.word	-1
int_const7:
	.word	2
	.word	4
	.word	Int_dispTab
	.word	9
	.word	-1
int_const6:
	.word	2
	.word	4
	.word	Int_dispTab
	.word	13
	.word	-1
int_const5:
	.word	2
	.word	4
	.word	Int_dispTab
	.word	2
	.word	-1
int_const4:
	.word	2
	.word	4
	.word	Int_dispTab
	.word	3
	.word	-1
int_const3:
	.word	2
	.word	4
	.word	Int_dispTab
	.word	6
	.word	-1
int_const2:
	.word	2
	.word	4
	.word	Int_dispTab
	.word	4
	.word	-1
int_const1:
	.word	2
	.word	4
	.word	Int_dispTab
	.word	10
	.word	-1
int_const0:
	.word	2
	.word	4
	.word	Int_dispTab
	.word	0
	.word	-1
bool_const0:
	.word	3
	.word	4
	.word	Bool_dispTab
	.word	0
	.word	-1
bool_const1:
	.word	3
	.word	4
	.word	Bool_dispTab
	.word	1
class_nameTab:
	.word	str_const2
	.word	str_const3
	.word	str_const4
	.word	str_const5
	.word	str_const6
	.word	str_const7
	.word	str_const8
	.word	str_const9
	.word	str_const10
	.word	str_const11
	.word	str_const12
class_objTab:
	.word	Object_protObj
	.word	Object_init
	.word	IO_protObj
	.word	IO_init
	.word	Int_protObj
	.word	Int_init
	.word	Bool_protObj
	.word	Bool_init
	.word	String_protObj
	.word	String_init
	.word	E606wxzslv_protObj
	.word	E606wxzslv_init
	.word	RJ4F9vxUtb_protObj
	.word	RJ4F9vxUtb_init
	.word	HNX5IGq5yQ_protObj
	.word	HNX5IGq5yQ_init
	.word	CRys1Atcrp_protObj
	.word	CRys1Atcrp_init
	.word	Main_protObj
	.word	Main_init
	.word	UrfbZWZFyG_protObj
	.word	UrfbZWZFyG_init
_max_tag:
	.word	10
class_parentTab:
	.word	-2
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	5
	.word	6
	.word	7
	.word	7
	.word	9
class_attrTabTab:
	.word	Object_attrTab
	.word	IO_attrTab
	.word	Int_attrTab
	.word	Bool_attrTab
	.word	String_attrTab
	.word	E606wxzslv_attrTab
	.word	RJ4F9vxUtb_attrTab
	.word	HNX5IGq5yQ_attrTab
	.word	CRys1Atcrp_attrTab
	.word	Main_attrTab
	.word	UrfbZWZFyG_attrTab
Object_attrTab:
E606wxzslv_attrTab:
	.word	0
	.word	7
	.word	7
RJ4F9vxUtb_attrTab:
	.word	0
	.word	7
	.word	7
	.word	8
	.word	10
	.word	7
HNX5IGq5yQ_attrTab:
	.word	0
	.word	7
	.word	7
	.word	8
	.word	10
	.word	7
	.word	5
	.word	8
	.word	5
Main_attrTab:
	.word	0
	.word	7
	.word	7
	.word	8
	.word	10
	.word	7
	.word	5
	.word	8
	.word	5
	.word	10
	.word	8
	.word	4
UrfbZWZFyG_attrTab:
	.word	0
	.word	7
	.word	7
	.word	8
	.word	10
	.word	7
	.word	5
	.word	8
	.word	5
	.word	10
	.word	8
	.word	4
	.word	6
	.word	7
	.word	6
CRys1Atcrp_attrTab:
	.word	0
	.word	7
	.word	7
	.word	8
	.word	10
	.word	7
	.word	5
	.word	8
	.word	5
	.word	3
	.word	6
	.word	9
String_attrTab:
	.word	2
	.word	-2
Bool_attrTab:
	.word	-2
Int_attrTab:
	.word	-2
IO_attrTab:
Object_dispTab:
	.word	Object.abort
	.word	Object.type_name
	.word	Object.copy
E606wxzslv_dispTab:
	.word	Object.abort
	.word	Object.type_name
	.word	Object.copy
	.word	E606wxzslv.ceacI
	.word	E606wxzslv.ptWr_
	.word	E606wxzslv.vnTCI
RJ4F9vxUtb_dispTab:
	.word	Object.abort
	.word	Object.type_name
	.word	Object.copy
	.word	E606wxzslv.ceacI
	.word	E606wxzslv.ptWr_
	.word	E606wxzslv.vnTCI
	.word	RJ4F9vxUtb.hGfhe
	.word	RJ4F9vxUtb.oOsrb
HNX5IGq5yQ_dispTab:
	.word	Object.abort
	.word	Object.type_name
	.word	Object.copy
	.word	E606wxzslv.ceacI
	.word	E606wxzslv.ptWr_
	.word	E606wxzslv.vnTCI
	.word	RJ4F9vxUtb.hGfhe
	.word	RJ4F9vxUtb.oOsrb
	.word	HNX5IGq5yQ.jEkcz
	.word	HNX5IGq5yQ.crGjC
	.word	HNX5IGq5yQ.tPK_l
Main_dispTab:
	.word	Object.abort
	.word	Object.type_name
	.word	Object.copy
	.word	E606wxzslv.ceacI
	.word	E606wxzslv.ptWr_
	.word	E606wxzslv.vnTCI
	.word	RJ4F9vxUtb.hGfhe
	.word	RJ4F9vxUtb.oOsrb
	.word	HNX5IGq5yQ.jEkcz
	.word	HNX5IGq5yQ.crGjC
	.word	HNX5IGq5yQ.tPK_l
	.word	Main.main
UrfbZWZFyG_dispTab:
	.word	Object.abort
	.word	Object.type_name
	.word	Object.copy
	.word	E606wxzslv.ceacI
	.word	E606wxzslv.ptWr_
	.word	E606wxzslv.vnTCI
	.word	RJ4F9vxUtb.hGfhe
	.word	RJ4F9vxUtb.oOsrb
	.word	HNX5IGq5yQ.jEkcz
	.word	HNX5IGq5yQ.crGjC
	.word	HNX5IGq5yQ.tPK_l
	.word	Main.main
	.word	UrfbZWZFyG.bV_Y4
	.word	UrfbZWZFyG.y7FJT
CRys1Atcrp_dispTab:
	.word	Object.abort
	.word	Object.type_name
	.word	Object.copy
	.word	E606wxzslv.ceacI
	.word	E606wxzslv.ptWr_
	.word	E606wxzslv.vnTCI
	.word	RJ4F9vxUtb.hGfhe
	.word	RJ4F9vxUtb.oOsrb
	.word	HNX5IGq5yQ.jEkcz
	.word	HNX5IGq5yQ.crGjC
	.word	HNX5IGq5yQ.tPK_l
	.word	CRys1Atcrp.sFUDI
	.word	CRys1Atcrp.jWsvt
String_dispTab:
	.word	Object.abort
	.word	Object.type_name
	.word	Object.copy
	.word	String.length
	.word	String.concat
	.word	String.substr
Bool_dispTab:
	.word	Object.abort
	.word	Object.type_name
	.word	Object.copy
Int_dispTab:
	.word	Object.abort
	.word	Object.type_name
	.word	Object.copy
IO_dispTab:
	.word	Object.abort
	.word	Object.type_name
	.word	Object.copy
	.word	IO.out_string
	.word	IO.out_int
	.word	IO.in_string
	.word	IO.in_int
	.word	-1
Object_protObj:
	.word	0
	.word	3
	.word	Object_dispTab
	.word	-1
E606wxzslv_protObj:
	.word	5
	.word	6
	.word	E606wxzslv_dispTab
	.word	0
	.word	0
	.word	0
	.word	-1
RJ4F9vxUtb_protObj:
	.word	6
	.word	9
	.word	RJ4F9vxUtb_dispTab
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	-1
HNX5IGq5yQ_protObj:
	.word	7
	.word	12
	.word	HNX5IGq5yQ_dispTab
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	-1
Main_protObj:
	.word	9
	.word	15
	.word	Main_dispTab
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	str_const13
	.word	-1
UrfbZWZFyG_protObj:
	.word	10
	.word	18
	.word	UrfbZWZFyG_dispTab
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	str_const13
	.word	0
	.word	0
	.word	0
	.word	-1
CRys1Atcrp_protObj:
	.word	8
	.word	15
	.word	CRys1Atcrp_dispTab
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	bool_const0
	.word	0
	.word	0
	.word	-1
String_protObj:
	.word	4
	.word	5
	.word	String_dispTab
	.word	int_const0
	.word	0
	.word	-1
Bool_protObj:
	.word	3
	.word	4
	.word	Bool_dispTab
	.word	0
	.word	-1
Int_protObj:
	.word	2
	.word	4
	.word	Int_dispTab
	.word	0
	.word	-1
IO_protObj:
	.word	1
	.word	3
	.word	IO_dispTab
	.globl	heap_start
heap_start:
	.word	0
	.text
	.globl	Main_init
	.globl	Int_init
	.globl	String_init
	.globl	Bool_init
	.globl	Main.main
Object_init:
	addiu	$sp $sp -12
	sw	$fp 12($sp)
	sw	$s0 8($sp)
	sw	$ra 4($sp)
	addiu	$fp $sp 16
	move	$s0 $a0
	move	$a0 $s0
	lw	$fp 12($sp)
	lw	$s0 8($sp)
	lw	$ra 4($sp)
	addiu	$sp $sp 12
	jr	$ra	
E606wxzslv_init:
	addiu	$sp $sp -12
	sw	$fp 12($sp)
	sw	$s0 8($sp)
	sw	$ra 4($sp)
	addiu	$fp $sp 16
	move	$s0 $a0
	jal	Object_init
	la	$a0 Object_protObj
	jal	Object.copy
	jal	Object_init
	sw	$a0 12($s0)
	la	$a0 HNX5IGq5yQ_protObj
	jal	Object.copy
	jal	HNX5IGq5yQ_init
	sw	$a0 16($s0)
	la	$a0 HNX5IGq5yQ_protObj
	jal	Object.copy
	jal	HNX5IGq5yQ_init
	sw	$a0 20($s0)
	move	$a0 $s0
	lw	$fp 12($sp)
	lw	$s0 8($sp)
	lw	$ra 4($sp)
	addiu	$sp $sp 12
	jr	$ra	
RJ4F9vxUtb_init:
	addiu	$sp $sp -12
	sw	$fp 12($sp)
	sw	$s0 8($sp)
	sw	$ra 4($sp)
	addiu	$fp $sp 16
	move	$s0 $a0
	jal	E606wxzslv_init
	la	$a0 CRys1Atcrp_protObj
	jal	Object.copy
	jal	CRys1Atcrp_init
	sw	$a0 24($s0)
	la	$a0 UrfbZWZFyG_protObj
	jal	Object.copy
	jal	UrfbZWZFyG_init
	sw	$a0 28($s0)
	la	$a0 HNX5IGq5yQ_protObj
	jal	Object.copy
	jal	HNX5IGq5yQ_init
	sw	$a0 32($s0)
	move	$a0 $s0
	lw	$fp 12($sp)
	lw	$s0 8($sp)
	lw	$ra 4($sp)
	addiu	$sp $sp 12
	jr	$ra	
HNX5IGq5yQ_init:
	addiu	$sp $sp -12
	sw	$fp 12($sp)
	sw	$s0 8($sp)
	sw	$ra 4($sp)
	addiu	$fp $sp 16
	move	$s0 $a0
	jal	RJ4F9vxUtb_init
	la	$a0 E606wxzslv_protObj
	jal	Object.copy
	jal	E606wxzslv_init
	sw	$a0 36($s0)
	la	$a0 CRys1Atcrp_protObj
	jal	Object.copy
	jal	CRys1Atcrp_init
	sw	$a0 40($s0)
	la	$a0 E606wxzslv_protObj
	jal	Object.copy
	jal	E606wxzslv_init
	sw	$a0 44($s0)
	move	$a0 $s0
	lw	$fp 12($sp)
	lw	$s0 8($sp)
	lw	$ra 4($sp)
	addiu	$sp $sp 12
	jr	$ra	
Main_init:
	addiu	$sp $sp -12
	sw	$fp 12($sp)
	sw	$s0 8($sp)
	sw	$ra 4($sp)
	addiu	$fp $sp 16
	move	$s0 $a0
	jal	HNX5IGq5yQ_init
	la	$a0 UrfbZWZFyG_protObj
	jal	Object.copy
	jal	UrfbZWZFyG_init
	sw	$a0 48($s0)
	la	$a0 CRys1Atcrp_protObj
	jal	Object.copy
	jal	CRys1Atcrp_init
	sw	$a0 52($s0)
	la	$a0 String_protObj
	jal	Object.copy
	jal	String_init
	sw	$a0 56($s0)
	move	$a0 $s0
	lw	$fp 12($sp)
	lw	$s0 8($sp)
	lw	$ra 4($sp)
	addiu	$sp $sp 12
	jr	$ra	
UrfbZWZFyG_init:
	addiu	$sp $sp -12
	sw	$fp 12($sp)
	sw	$s0 8($sp)
	sw	$ra 4($sp)
	addiu	$fp $sp 16
	move	$s0 $a0
	jal	Main_init
	la	$a0 RJ4F9vxUtb_protObj
	jal	Object.copy
	jal	RJ4F9vxUtb_init
	sw	$a0 60($s0)
	la	$a0 HNX5IGq5yQ_protObj
	jal	Object.copy
	jal	HNX5IGq5yQ_init
	sw	$a0 64($s0)
	la	$a0 RJ4F9vxUtb_protObj
	jal	Object.copy
	jal	RJ4F9vxUtb_init
	sw	$a0 68($s0)
	move	$a0 $s0
	lw	$fp 12($sp)
	lw	$s0 8($sp)
	lw	$ra 4($sp)
	addiu	$sp $sp 12
	jr	$ra	
CRys1Atcrp_init:
	addiu	$sp $sp -12
	sw	$fp 12($sp)
	sw	$s0 8($sp)
	sw	$ra 4($sp)
	addiu	$fp $sp 16
	move	$s0 $a0
	jal	HNX5IGq5yQ_init
	la	$a0 Bool_protObj
	jal	Object.copy
	jal	Bool_init
	sw	$a0 48($s0)
	la	$a0 RJ4F9vxUtb_protObj
	jal	Object.copy
	jal	RJ4F9vxUtb_init
	sw	$a0 52($s0)
	la	$a0 Main_protObj
	jal	Object.copy
	jal	Main_init
	sw	$a0 56($s0)
	move	$a0 $s0
	lw	$fp 12($sp)
	lw	$s0 8($sp)
	lw	$ra 4($sp)
	addiu	$sp $sp 12
	jr	$ra	
String_init:
	addiu	$sp $sp -12
	sw	$fp 12($sp)
	sw	$s0 8($sp)
	sw	$ra 4($sp)
	addiu	$fp $sp 16
	move	$s0 $a0
	jal	Object_init
	move	$a0 $s0
	lw	$fp 12($sp)
	lw	$s0 8($sp)
	lw	$ra 4($sp)
	addiu	$sp $sp 12
	jr	$ra	
Bool_init:
	addiu	$sp $sp -12
	sw	$fp 12($sp)
	sw	$s0 8($sp)
	sw	$ra 4($sp)
	addiu	$fp $sp 16
	move	$s0 $a0
	jal	Object_init
	move	$a0 $s0
	lw	$fp 12($sp)
	lw	$s0 8($sp)
	lw	$ra 4($sp)
	addiu	$sp $sp 12
	jr	$ra	
Int_init:
	addiu	$sp $sp -12
	sw	$fp 12($sp)
	sw	$s0 8($sp)
	sw	$ra 4($sp)
	addiu	$fp $sp 16
	move	$s0 $a0
	jal	Object_init
	move	$a0 $s0
	lw	$fp 12($sp)
	lw	$s0 8($sp)
	lw	$ra 4($sp)
	addiu	$sp $sp 12
	jr	$ra	
IO_init:
	addiu	$sp $sp -12
	sw	$fp 12($sp)
	sw	$s0 8($sp)
	sw	$ra 4($sp)
	addiu	$fp $sp 16
	move	$s0 $a0
	jal	Object_init
	move	$a0 $s0
	lw	$fp 12($sp)
	lw	$s0 8($sp)
	lw	$ra 4($sp)
	addiu	$sp $sp 12
	jr	$ra	
E606wxzslv.ceacI:
	addiu	$sp $sp -12
	sw	$fp 12($sp)
	sw	$s0 8($sp)
	sw	$ra 4($sp)
	addiu	$fp $sp 16
	move	$s0 $a0
	la	$a0 E606wxzslv_protObj
	jal	Object.copy
	jal	E606wxzslv_init
	lw	$fp 12($sp)
	lw	$s0 8($sp)
	lw	$ra 4($sp)
	addiu	$sp $sp 32
	jr	$ra	
E606wxzslv.ptWr_:
	addiu	$sp $sp -12
	sw	$fp 12($sp)
	sw	$s0 8($sp)
	sw	$ra 4($sp)
	addiu	$fp $sp 16
	move	$s0 $a0
	la	$a0 Int_protObj
	jal	Object.copy
	jal	Int_init
	lw	$fp 12($sp)
	lw	$s0 8($sp)
	lw	$ra 4($sp)
	addiu	$sp $sp 32
	jr	$ra	
E606wxzslv.vnTCI:
	addiu	$sp $sp -12
	sw	$fp 12($sp)
	sw	$s0 8($sp)
	sw	$ra 4($sp)
	addiu	$fp $sp 16
	move	$s0 $a0
	la	$a0 IO_protObj
	jal	Object.copy
	jal	IO_init
	lw	$fp 12($sp)
	lw	$s0 8($sp)
	lw	$ra 4($sp)
	addiu	$sp $sp 32
	jr	$ra	
RJ4F9vxUtb.hGfhe:
	addiu	$sp $sp -12
	sw	$fp 12($sp)
	sw	$s0 8($sp)
	sw	$ra 4($sp)
	addiu	$fp $sp 16
	move	$s0 $a0
	la	$a0 Object_protObj
	jal	Object.copy
	jal	Object_init
	lw	$fp 12($sp)
	lw	$s0 8($sp)
	lw	$ra 4($sp)
	addiu	$sp $sp 32
	jr	$ra	
RJ4F9vxUtb.oOsrb:
	addiu	$sp $sp -12
	sw	$fp 12($sp)
	sw	$s0 8($sp)
	sw	$ra 4($sp)
	addiu	$fp $sp 16
	move	$s0 $a0
	la	$a0 RJ4F9vxUtb_protObj
	jal	Object.copy
	jal	RJ4F9vxUtb_init
	lw	$fp 12($sp)
	lw	$s0 8($sp)
	lw	$ra 4($sp)
	addiu	$sp $sp 32
	jr	$ra	
HNX5IGq5yQ.jEkcz:
	addiu	$sp $sp -12
	sw	$fp 12($sp)
	sw	$s0 8($sp)
	sw	$ra 4($sp)
	addiu	$fp $sp 16
	move	$s0 $a0
	la	$a0 HNX5IGq5yQ_protObj
	jal	Object.copy
	jal	HNX5IGq5yQ_init
	lw	$fp 12($sp)
	lw	$s0 8($sp)
	lw	$ra 4($sp)
	addiu	$sp $sp 32
	jr	$ra	
HNX5IGq5yQ.crGjC:
	addiu	$sp $sp -12
	sw	$fp 12($sp)
	sw	$s0 8($sp)
	sw	$ra 4($sp)
	addiu	$fp $sp 16
	move	$s0 $a0
	la	$a0 Main_protObj
	jal	Object.copy
	jal	Main_init
	lw	$fp 12($sp)
	lw	$s0 8($sp)
	lw	$ra 4($sp)
	addiu	$sp $sp 32
	jr	$ra	
HNX5IGq5yQ.tPK_l:
	addiu	$sp $sp -12
	sw	$fp 12($sp)
	sw	$s0 8($sp)
	sw	$ra 4($sp)
	addiu	$fp $sp 16
	move	$s0 $a0
	la	$a0 Main_protObj
	jal	Object.copy
	jal	Main_init
	lw	$fp 12($sp)
	lw	$s0 8($sp)
	lw	$ra 4($sp)
	addiu	$sp $sp 32
	jr	$ra	
Main.main:
	addiu	$sp $sp -12
	sw	$fp 12($sp)
	sw	$s0 8($sp)
	sw	$ra 4($sp)
	addiu	$fp $sp 16
	move	$s0 $a0
	la	$a0 UrfbZWZFyG_protObj
	jal	Object.copy
	jal	UrfbZWZFyG_init
	lw	$fp 12($sp)
	lw	$s0 8($sp)
	lw	$ra 4($sp)
	addiu	$sp $sp 12
	jr	$ra	
UrfbZWZFyG.bV_Y4:
	addiu	$sp $sp -12
	sw	$fp 12($sp)
	sw	$s0 8($sp)
	sw	$ra 4($sp)
	addiu	$fp $sp 16
	move	$s0 $a0
	la	$a0 Main_protObj
	jal	Object.copy
	jal	Main_init
	lw	$fp 12($sp)
	lw	$s0 8($sp)
	lw	$ra 4($sp)
	addiu	$sp $sp 32
	jr	$ra	
UrfbZWZFyG.y7FJT:
	addiu	$sp $sp -12
	sw	$fp 12($sp)
	sw	$s0 8($sp)
	sw	$ra 4($sp)
	addiu	$fp $sp 16
	move	$s0 $a0
	la	$a0 RJ4F9vxUtb_protObj
	jal	Object.copy
	jal	RJ4F9vxUtb_init
	lw	$fp 12($sp)
	lw	$s0 8($sp)
	lw	$ra 4($sp)
	addiu	$sp $sp 32
	jr	$ra	
CRys1Atcrp.sFUDI:
	addiu	$sp $sp -12
	sw	$fp 12($sp)
	sw	$s0 8($sp)
	sw	$ra 4($sp)
	addiu	$fp $sp 16
	move	$s0 $a0
	la	$a0 HNX5IGq5yQ_protObj
	jal	Object.copy
	jal	HNX5IGq5yQ_init
	lw	$fp 12($sp)
	lw	$s0 8($sp)
	lw	$ra 4($sp)
	addiu	$sp $sp 32
	jr	$ra	
CRys1Atcrp.jWsvt:
	addiu	$sp $sp -12
	sw	$fp 12($sp)
	sw	$s0 8($sp)
	sw	$ra 4($sp)
	addiu	$fp $sp 16
	move	$s0 $a0
	la	$a0 Object_protObj
	jal	Object.copy
	jal	Object_init
	lw	$fp 12($sp)
	lw	$s0 8($sp)
	lw	$ra 4($sp)
	addiu	$sp $sp 32
	jr	$ra	
