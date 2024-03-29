[loadplugin module="wuvorbis.dll"]

; @rclick call=false << hideMessage
; @rclick enabled=false

; hideMessage
;@rclick call=false

;@wait time=2000
;[iscript]
;var tmpLayer = new Layer(kag, kag.fore.base);
;var tmpFontList = tmpLayer.font.getList(fsfNoVertical);
;for(var i = 0; i < tmpFontList.length; i++) { dm(i+" = "+tmpFontList[i]); }
;[endscript]
;@s

; 右クリック使用不可
@rclick enabled=false

@wait time=100

; フラグ(tf.versionFlag は AfterInit.tjs で設定)
@eval exp="tf.from_collection = 0"
@eval exp="tf.title_debug = 0"


;===============================================================================

; 言語選択リクエスト
; @eval exp="tf.selectLang = false"
; @eval exp="tf.selectLang = true" cond="sf.lang == void"

; 言語処理
@eval exp="tf.selectLang = false"
@eval exp="tf.lang = sf.lang"
@eval exp="tf.lang = 1; sf.lang = 1; tf.selectLang = true;" cond="tf.lang === void || tf.lang === '' || tf.lang < 0 || tf.lang > 3"

@eval exp="tf.selectLang = true;" cond="sf.selectLangRetry != void && sf.selectLangRetry && !tf.rebootFlag"

; 言語別フォント設定（メッセージ系）
@eval exp="tf.langFontFace = 'Noto Sans JP'" cond="tf.lang == 0"
@eval exp="tf.langFontFace = 'Noto Sans JP Medium'" cond="tf.lang == 1"
@eval exp="tf.langFontFace = 'Noto Sans TC'" cond="tf.lang == 2"
@eval exp="tf.langFontFace = 'Noto Sans SC'" cond="tf.lang == 3"

; 言語別フォント設定（UI系）
@eval exp="tf.langUiFontFace = 'Noto Serif JP'" cond="tf.lang == 0"
@eval exp="tf.langUiFontFace = 'Noto Serif JP'" cond="tf.lang == 1"
@eval exp="tf.langUiFontFace = 'Noto Serif TC'" cond="tf.lang == 2"
@eval exp="tf.langUiFontFace = 'Noto Serif SC'" cond="tf.lang == 3"

; コマンドからパラメータ読み込み&保存
; @eval exp="tf.tmpLang = System.getArgument('-lang')"
; @eval exp="tf.lang = tf.tmpLang; sf.lang = tf.lang;" cond="tf.tmpLang != void && tf.tmpLang >= 0 && tf.tmpLang <= 3"

; 言語処理
; @eval exp="tf.lang = +System.getArgument('-lang')"
; @eval exp="tf.lang = 0" cond="tf.lang == void || tf.lang == '' || tf.lang < 0 || tf.lang > 3"

; 一時変数デフォルト値(下でシステム変数から書き換える)
@eval exp="tf.nowMessageLang = ''"
@eval exp="tf.textSize = 1"
@eval exp="tf.edgeSize = 2"
@eval exp="tf.textViewSpeed = 50"
@eval exp="tf.msgWindowType = 1"
@eval exp="tf.msgWindowOpacity = 127"

@eval exp="tf.autoModeEnabled = 0"
@eval exp="tf.autoModeBaseTime = 3000"
@eval exp="tf.autoModeChTime = 12"
@eval exp="tf.autoModeVoiceTime = 125"

; Volumeはデフォルト値が必要
@eval exp="tf.defaultVolumeMaster = 75"
@eval exp="tf.defaultVolumeBgm = 50"
@eval exp="tf.defaultVolumeSe = 50"
@eval exp="tf.defaultVolumeVoice = 50"
@eval exp="tf.defaultVolumeMovie = 50"

@eval exp="tf.volumeMaster = tf.defaultVolumeMaster"
@eval exp="tf.volumeBgm = tf.defaultVolumeBgm"
@eval exp="tf.volumeSe = tf.defaultVolumeSe"
@eval exp="tf.volumeVoice = tf.defaultVolumeVoice"
@eval exp="tf.volumeMovie = tf.defaultVolumeMovie"


@eval exp="tf.textSize = sf.configTextSize" cond="sf.configTextSize !== void"
@eval exp="tf.edgeSize = sf.configEdgeSize" cond="sf.configEdgeSize !== void"
@eval exp="tf.textViewSpeed = sf.textViewSpeed" cond="sf.textViewSpeed !== void"
@eval exp="tf.msgWindowType = sf.configMsgWindowType" cond="sf.configMsgWindowType !== void"
@eval exp="tf.msgWindowOpacity = sf.configMsgWindowOpacity" cond="sf.configMsgWindowOpacity !== void"

@eval exp="tf.autoModeBaseTime = sf.autoModeBaseTime" cond="sf.autoModeBaseTime !== void"
@eval exp="tf.autoModeChTime = sf.autoModeChTime" cond="sf.autoModeChTime !== void"
@eval exp="tf.autoModeVoiceTime = sf.autoModeVoiceTime" cond="sf.autoModeVoiceTime !== void"

@eval exp="tf.volumeMaster = sf.configVolumeMaster" cond="sf.configVolumeMaster !== void"
@eval exp="tf.volumeBgm = sf.configVolumeBgm" cond="sf.configVolumeBgm !== void"
@eval exp="tf.volumeSe = sf.configVolumeSe" cond="sf.configVolumeSe !== void"
@eval exp="tf.volumeVoice = sf.configVolumeVoice" cond="sf.configVolumeVoice !== void"
@eval exp="tf.volumeMovie = sf.configVolumeMovie" cond="sf.configVolumeMovie !== void"

@eval exp="tf.volumeBgm = 0" cond="sf.configVolumeBgmMute !== void && sf.configVolumeBgmMute == 0"
@eval exp="tf.volumeSe = 0" cond="sf.configVolumeSeMute !== void && sf.configVolumeSeMute == 0"
@eval exp="tf.volumeVoice = 0" cond="sf.configVolumeVoiceMute !== void && sf.configVolumeVoiceMute == 0"

@eval exp="tf.volumeMovie = int(tf.volumeMovie * (tf.volumeMaster / 100.0))"


; ゲーム変数(保存)
@eval exp="f.nowDrawTarget = 'fore'"
@eval exp="f.oldBgCutIn = %[]"
@eval exp="f.fox_face = 0"
@eval exp="f.obj_move = %[]"
@eval exp="f.charaClothes = %[]"
@eval exp="f.msgPosDatas = []"
@eval exp="f.msgWindowColorType = 0"

@eval exp="f.bgPos = %[]"
@eval exp="f.charaPoses = []"
@eval exp="for(var i = 0; i <= 16; i++) { f.charaPoses[i] = %[]; }"

; システム変数(保存)
@eval exp="sf.clearFlag = 0" cond="sf.clearFlag == void"
@eval exp="sf.clearFlags = []; for(var i = 0; i <= 3; i++){ sf.clearFlags[i] = false; };" cond="sf.clearFlags == void"

; カメラ座標
@eval exp="f.cameraX = 0.0"
@eval exp="f.cameraY = 0.0"
@eval exp="f.cameraZ = 0.0"
@eval exp="f.cameraRotate = 0.0"

; BGM音量設定反映
@bgmopt gvolume=&tf.volumeMaster volume=&tf.volumeBgm

; 言語別翻訳ファイル読込
@eval exp="removeAutoPathXp3('lang/lang_ja/', 'data')"
@eval exp="removeAutoPathXp3('lang/lang_en/', 'data')"
@eval exp="removeAutoPathXp3('lang/lang_zhtw/', 'data')"
@eval exp="removeAutoPathXp3('lang/lang_zhcn/', 'data')"

@eval exp="removeAutoPathXp3('lang/lang_ja/', 'patch')"
@eval exp="removeAutoPathXp3('lang/lang_ja/', 'patch2')"
@eval exp="removeAutoPathXp3('lang/lang_en/', 'patch')"
@eval exp="removeAutoPathXp3('lang/lang_zhtw/', 'patch')"
@eval exp="removeAutoPathXp3('lang/lang_zhcn/', 'patch')"

@eval exp="addAutoPathXp3('lang/lang_ja/', 'data')" cond="tf.lang == 0"
@eval exp="addAutoPathXp3('lang/lang_en/', 'data')" cond="tf.lang == 1"
@eval exp="addAutoPathXp3('lang/lang_zhtw/', 'data')" cond="tf.lang == 2"
@eval exp="addAutoPathXp3('lang/lang_zhcn/', 'data')" cond="tf.lang == 3"

@eval exp="addAutoPathXp3('lang/lang_ja/', 'patch')" cond="tf.lang == 0"
@eval exp="addAutoPathXp3('lang/lang_ja/', 'patch2')" cond="tf.lang == 0"
@eval exp="addAutoPathXp3('lang/lang_en/', 'patch')" cond="tf.lang == 1"
@eval exp="addAutoPathXp3('lang/lang_zhtw/', 'patch')" cond="tf.lang == 2"
@eval exp="addAutoPathXp3('lang/lang_zhcn/', 'patch')" cond="tf.lang == 3"

@call storage="_nameLang.ks"
@call storage="_selectLang.ks"
@call storage="_systemLang.ks"
@call storage="_titleLang.ks"
@call storage="_musicLang.ks"

[iscript]
function f_get_nameLang(id) {
	if (id == void || id == "") {
		return "";
	}
	return tf._nameLang[id];
}
function f_get_selectLang(id) {
	if (id == void || id == "") {
		return "";
	}
	return tf._selectLang[id];
}
function f_get_systemLang(id) {
	if (id == void || id == "") {
		return "";
	}
	return tf._systemLang[id];
}
function f_get_titleLang(id) {
	if (id == void || id == "") {
		return "";
	}
	return tf._titleLang[id];
}
function f_get_musicLang(id) {
	if (id == void || id == "") {
		return "";
	}
	return tf._musicLang[id];
}
function f_check_messageLang(file, id) {
	if (tf.nowMessageLang != file) {
		if (!Storages.isExistentStorage(file+"_lang.tjs")) return false;
		KAGLoadScript(file+"_lang.tjs");
	}
	if (tf.langScenario[id] === void) return false;
	return true;
}
function f_get_messageLang(file, id) {
	if (tf.nowMessageLang != file) {
		KAGLoadScript(file+"_lang.tjs");
	}
	return tf.langScenario[id];
}
function f_get_imgPlusLangName(file) {
	if (tf.lang == 0) return file + "_ja";
	if (tf.lang == 1) return file + "_en";
	if (tf.lang == 2) return file + "_zhtw";
	if (tf.lang == 3) return file + "_zhcn";
}
[endscript]


@call storage="_macro.ks"
@call storage="_font.ks"
@call storage="_grad_text.ks"
@call storage="_message.ks"
@call storage="_windowMessage.ks"
@call storage="_menu.ks"
@call storage="_bg.ks"
@call storage="_music.ks"
@call storage="_video.ks"
@call storage="_disp.ks"
@call storage="_standCharaName.ks"
@call storage="_staff.ks"
@call storage="_other.ks"

@call storage="SaveAnywhere.ks"
@call storage="YesNoDialogLayer.ks"
@call storage="CutInPlugin.ks"
@call storage="SnowPlus.ks"
@call storage="CameraLayer.ks"
@call storage="CtrlSkip.ks"

[iscript]
ImageDialogLayerPlugin_obj.setOptions(%[
	movecursor : false,
	font : %[
		face : tf.uiFont.face,
		bold : tf.uiFont.bold,
		height : 30,
		color :  tf.uiFont.color,
		
	],
	shadow : %[
		level : tf.uiFont.shadowLevel,
		color : tf.uiFont.shadowColor,
		width : tf.uiFont.shadowWidth,
		offsetx : 0,
		offsety : 0
	]
]);
[endscript]

; 無音SEループ再生
@playse buf=3 storage="sys0000.ogg" loop=true

;===============================================================================

; 言語選択Popup
@selectlanguage cond="tf.selectLang == true"

@jump storage="logo.ks"