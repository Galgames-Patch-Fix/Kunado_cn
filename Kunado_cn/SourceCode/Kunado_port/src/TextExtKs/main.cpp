#include <string>
#include <vector>
#include <map>
#include <unordered_map>

#include "../../lib/Rut/RxFile.h"
#include "../../lib/Rut/RxPath.h"
#include "../../lib/Rut/RxConsole.h"
#include "../../lib/RxJson/RxJson.h"

using namespace Rut;


class TextStruct
{
public:
	std::wstring m_wsRaw;
	std::wstring m_wsTra;
};


std::wstring GetName(std::wstring& wsLine)
{
	if (wsLine[0] != L';')
	{
		RxConsole::Put("Not Line!\n");
		return L"";
	}

	size_t pos = wsLine.find(L"\">>\"");
	if (pos != std::wstring::npos)
	{
		return wsLine.substr(3, pos - 3);
	}

	RxConsole::Put("Get GetName Error!\n");
	return L"";
}

std::wstring GetMsg(std::wstring& wsLine)
{
	if (wsLine[0] != L';')
	{
		RxConsole::Put("Not Line!\n");
		return L"";
	}

	size_t pos = wsLine.find(L"\">>\"");
	if (pos != std::wstring::npos)
	{
		pos += std::size(L"\">>\"") - 1;
		std::wstring txt = wsLine.substr(pos, wsLine.size() - pos - 1);
		if (txt.find(L"\")") != std::wstring::npos)
		{
			txt.pop_back();
			txt.pop_back();
		}
		return txt;
	}

	RxConsole::Put("Get GetLineName Error!\n");
	return L"";
}

std::wstring GetParameterValue(std::wstring& wsCode,  std::wstring wsPara)
{
	if (wsCode[0] != L'@')
	{
		RxConsole::Put("Not Code!\n");
		return L"";
	}

	size_t endPos = 0;
	size_t begPos = wsCode.find(wsPara);
	if (begPos != std::wstring::npos)
	{
		begPos += wsPara.size() + 1;
		endPos = wsCode.find(L"\"", begPos);
		if (endPos != std::wstring::npos)
		{
			return wsCode.substr(begPos, endPos - begPos);
		}
	}

	RxConsole::Put("Get Parameter Value Error!\n");
	return L"";
}



bool ReadTable(std::wstring wsFileName, std::unordered_map<std::wstring, std::wstring>& mapLine)
{
	RxFile::Text wifText{ wsFileName, RIO_READ, RFM_UTF8 };
	std::vector<std::wstring> text_list;
	wifText.ReadAllLine(text_list);

	for (auto ite = text_list.begin(); ite != text_list.end(); ite++)
	{
		std::wstring& line = *ite;
		size_t pos = line.find(L"\" => \"");
		if (pos != std::wstring::npos)
		{
			line.erase(line.size() - 2, 2);
			std::wstring mes_no = line.substr(2, pos - 2);
			std::wstring text = line.substr(pos + 6);
			mapLine.insert(std::pair<std::wstring, std::wstring>(mes_no, text));
		}
	}

	return true;
}

bool ReplaceText(
	std::wstring wsFileName,
	std::vector<TextStruct>& vecText,
	std::unordered_map<std::wstring, std::wstring>& mapTraText,
	std::unordered_map<std::wstring, std::wstring>& mapTraName,
	std::unordered_map<std::wstring, std::wstring>& mapTraSelect,
	std::unordered_map<std::wstring, std::wstring>& mapTraSelect2,
	std::unordered_map<std::wstring, std::wstring>& mapTraTitle,
	std::unordered_map<std::wstring, std::wstring>& mapTraTitle2)
{
	RxFile::Text wifText{ wsFileName, RIO_READ, RFM_UTF8 };

	std::vector<std::wstring> text_list;
	wifText.ReadAllLine(text_list);

	TextStruct MsgStruct;
	TextStruct NameStruct;
	TextStruct SelectStruct;
	TextStruct TitleStruct;
	for (auto ite = text_list.begin(); ite != text_list.end(); ite++)
	{
		std::wstring& line = *ite;
		if (line.find(L"; \"") == 0)
		{
			NameStruct.m_wsRaw = GetName(line);
			MsgStruct.m_wsRaw = GetMsg(line);

			ite++;
			line = *ite;

			MsgStruct.m_wsTra = mapTraText.find(GetParameterValue(line, L"mes_no="))->second;
			NameStruct.m_wsTra = mapTraName.find(GetParameterValue(line, L"name_no="))->second;

			vecText.emplace_back(MsgStruct);
			vecText.emplace_back(NameStruct);
		}

		if (line.find(L"@api_set_message_select") == 0)
		{
			std::wstring kx = GetParameterValue(line, L"s0=");
			SelectStruct.m_wsRaw = mapTraSelect2.find(kx)->second;
			SelectStruct.m_wsTra = mapTraSelect.find(kx)->second;

			vecText.emplace_back(SelectStruct);
			continue;
		}

		if (line.find(L"@api_set_window_title_create") == 0)
		{
			std::wstring kt = GetParameterValue(line, L"title=");
			TitleStruct.m_wsRaw = mapTraTitle2.find(kt)->second;
			TitleStruct.m_wsTra = mapTraTitle.find(kt)->second;

			vecText.emplace_back(TitleStruct);
			continue;
		}
	}

	return true;
}

bool WriteText(std::wstring wsFileName, std::vector<TextStruct>& vecText)
{
	RxFile::Text wofText{ wsFileName, RIO_WRITE, RFM_UTF8 };

	for (auto& ite : vecText)
	{
		if (!ite.m_wsRaw.empty())
		{
			wofText << L"Raw:" + ite.m_wsRaw + L"\n";
			wofText << L"Tra:" + ite.m_wsTra + L"\n\n";
		}
	}

	return true;
}

void ExtractTexts(std::wstring wsTableKsPath, std::wstring wsIndexKsPath, std::wstring wsOutPath)
{
	std::unordered_map<std::wstring, std::wstring> mapTraTitle;
	ReadTable(wsTableKsPath + L"_titleLang.ks", mapTraTitle);

	std::unordered_map<std::wstring, std::wstring> mapTraTitle2;
	{
		mapTraTitle2[L"title_base"] = L"クナド国記";
		mapTraTitle2[L"title_natsu_h_01"] = L"- 夏姫Ｈ -";
		mapTraTitle2[L"title_haru_h_01"] = L"- 春姫Ｈ１ -";
		mapTraTitle2[L"title_haru_h_02"] = L"- 春姫Ｈ２ -";
		mapTraTitle2[L"title_haru_h_03"] = L"- 春姫Ｈ３ -";
		mapTraTitle2[L"title_yuri_h_01"] = L"- 優里Ｈ１ -";
		mapTraTitle2[L"title_yuri_h_02"] = L"- 優里Ｈ２ -";
		mapTraTitle2[L"title_yuri_h_03"] = L"- 優里Ｈ３ -";
		mapTraTitle2[L"title_twin_h_01"] = L"- 茜・葵Ｈ１ -";
		mapTraTitle2[L"title_twin_h_02"] = L"- 茜・葵Ｈ２ -";
		mapTraTitle2[L"title_twin_h_03"] = L"- 茜・葵Ｈ３ -";
		mapTraTitle2[L"title_twin_h_04"] = L"- 茜・葵Ｈ４ -";
		mapTraTitle2[L"title_tsubame_h_01"] = L"- 燕Ｈ -";
		mapTraTitle2[L"title_shiki_h_01"] = L"- 識Ｈ -";
		mapTraTitle2[L"title_main"] = L"- メイン -";
		mapTraTitle2[L"title_menu"] = L"- タイトルメニュー -";
		mapTraTitle2[L"title_haru_end"] = L"- エンドロール【春姫】 -";
		mapTraTitle2[L"title_yuri_end"] = L"- エンドロール【優里】 -";
		mapTraTitle2[L"title_twin_end"] = L"- エンドロール【双子】 -";
		mapTraTitle2[L"title_op_01"] = L"- オープニング -";
		mapTraTitle2[L"title_op_02"] = L"- セカンドオープニング -";
		mapTraTitle2[L"title_epilogue"] = L"- ２つのエピローグ -";
		mapTraTitle2[L"title_common_01"] = L"- 共通０１ -";
		mapTraTitle2[L"title_common_02"] = L"- 共通０２ -";
		mapTraTitle2[L"title_common_03"] = L"- 共通０３ -";
		mapTraTitle2[L"title_common_04"] = L"- 共通０４ -";
		mapTraTitle2[L"title_common_05"] = L"- 共通０５ -";
		mapTraTitle2[L"title_yuri_01"] = L"- 優里本筋０１ -";
		mapTraTitle2[L"title_yuri_02_01"] = L"- 優里本筋０２－０１ -";
		mapTraTitle2[L"title_yuri_02_02"] = L"- 優里本筋０２－０２ -";
		mapTraTitle2[L"title_yuri_03"] = L"- 優里本筋０３ -";
		mapTraTitle2[L"title_yuri_04"] = L"- 優里本筋０４ -";
		mapTraTitle2[L"title_yuri_05"] = L"- 優里本筋０５ -";
		mapTraTitle2[L"title_yuri_06"] = L"- 優里本筋０６ -";
		mapTraTitle2[L"title_yuri_07"] = L"- 優里本筋０７ -";
		mapTraTitle2[L"title_yuri_08_01"] = L"- 優里本筋０８－０１ -";
		mapTraTitle2[L"title_yuri_08_02"] = L"- 優里本筋０８－０２ -";
		mapTraTitle2[L"title_yuri_other_01"] = L"- 優里派生０１ -";
		mapTraTitle2[L"title_yuri_other_02"] = L"- 優里派生０２ -";
		mapTraTitle2[L"title_yuri_other_03"] = L"- 優里派生０３ -";
		mapTraTitle2[L"title_yuri_other_04"] = L"- 優里派生０４ -";
		mapTraTitle2[L"title_yuri_other_05"] = L"- 優里派生０５ -";
		mapTraTitle2[L"title_yuri_epilogue"] = L"- 優里エピローグ -";
		mapTraTitle2[L"title_twin_01_01"] = L"- 双子本筋０１－０１ -";
		mapTraTitle2[L"title_twin_01_02"] = L"- 双子本筋０１－０２ -";
		mapTraTitle2[L"title_twin_02"] = L"- 双子本筋０２ -";
		mapTraTitle2[L"title_twin_03"] = L"- 双子本筋０３ -";
		mapTraTitle2[L"title_twin_04"] = L"- 双子本筋０４ -";
		mapTraTitle2[L"title_twin_05"] = L"- 双子本筋０５ -";
		mapTraTitle2[L"title_twin_06_01"] = L"- 双子本筋０６－０１ -";
		mapTraTitle2[L"title_twin_06_02"] = L"- 双子本筋０６－０２ -";
		mapTraTitle2[L"title_twin_other_01"] = L"- 双子派生０１ -";
		mapTraTitle2[L"title_twin_other_02"] = L"- 双子派生０２ -";
		mapTraTitle2[L"title_twin_other_03"] = L"- 双子派生０３ -";
		mapTraTitle2[L"title_twin_other_04"] = L"- 双子派生０４ -";
		mapTraTitle2[L"title_twin_other_05"] = L"- 双子派生０５ -";
		mapTraTitle2[L"title_twin_other_06"] = L"- 双子派生０６ -";
		mapTraTitle2[L"title_twin_epilogue"] = L"- 双子エピローグ -";
		mapTraTitle2[L"title_haru_01"] = L"- 春姫本筋０１ -";
		mapTraTitle2[L"title_haru_02"] = L"- 春姫本筋０２ -";
		mapTraTitle2[L"title_haru_03"] = L"- 春姫本筋０３ -";
		mapTraTitle2[L"title_haru_04"] = L"- 春姫本筋０４ -";
		mapTraTitle2[L"title_haru_05"] = L"- 春姫本筋０５ -";
		mapTraTitle2[L"title_haru_06"] = L"- 春姫本筋０６ -";
		mapTraTitle2[L"title_haru_07_01"] = L"- 春姫本筋０７－０１ -";
		mapTraTitle2[L"title_haru_07_02"] = L"- 春姫本筋０７－０２ -";
		mapTraTitle2[L"title_haru_after_01"] = L"- アフターストーリー春姫１ -";
		mapTraTitle2[L"title_haru_after_03"] = L"- アフターストーリー春姫２ -";
		mapTraTitle2[L"title_twin_after_01"] = L"- アフターストーリー茜・葵 -";
		mapTraTitle2[L"title_tsubame_after_01"] = L"- パラレルストーリー燕 -";
		mapTraTitle2[L"title_shiki_after_01"] = L"- パラレルストーリー識 -";
	}


	std::unordered_map<std::wstring, std::wstring> mapTraSelect;
	ReadTable(wsTableKsPath + L"_selectLang.ks", mapTraSelect);

	std::unordered_map<std::wstring, std::wstring> mapTraSelect2;
	mapTraSelect2[L"select_yes"] = L"はい";
	mapTraSelect2[L"select_01"] = L"もっと先にある未来を見せる";
	mapTraSelect2[L"select_02"] = L"優里を心の底から愛している";
	mapTraSelect2[L"select_03"] = L"２人とも手を繋いで、さらに未来へ進む";
	mapTraSelect2[L"select_04"] = L"僕こそが２人を愛している";

	std::unordered_map<std::wstring, std::wstring> mapTraName;
	ReadTable(wsTableKsPath + L"_nameLang.ks", mapTraName);

	std::wstring out_path = wsOutPath + wsTableKsPath;

	std::vector<std::wstring> vecRawFiles;
	RxPath::CurFileNames(wsIndexKsPath, vecRawFiles);

	for (auto& fileName : vecRawFiles)
	{
		std::unordered_map<std::wstring, std::wstring> mapTraText;
		ReadTable(wsTableKsPath + fileName.substr(0, fileName.size() - 3) + L"_lang.tjs", mapTraText);

		std::vector<TextStruct> vecText;
		ReplaceText(wsIndexKsPath + fileName, vecText, mapTraText, mapTraName, mapTraSelect, mapTraSelect2, mapTraTitle, mapTraTitle2);

		for (auto& stx : vecText)
		{
			for (auto& cc : stx.m_wsRaw)
			{
				switch (cc)
				{
				case L'〜': cc = L'～'; break;
				}
			}
		}

		RxPath::MakeDirViaPath(out_path);
		WriteText(out_path + fileName + L".txt", vecText);
	}
}

void ImportToJson(std::wstring wsOrgJsonPath, std::wstring wsTextsPath, std::wstring wsOutJsonPath)
{
	RxPath::MakeDirViaPath(wsOutJsonPath);
	std::vector<std::wstring> json_list;
	RxPath::CurFileNames(wsOrgJsonPath, json_list);

	for (auto& json_file : json_list)
	{
		std::wstring json_path = wsOrgJsonPath + json_file;
		std::wstring txt_path = wsTextsPath + json_file.substr(0, json_file.size() - 9) + L".ks.txt";

		RxFile::Text wifText{ txt_path, RIO_READ, RFM_UTF8 };

		std::vector<TextStruct> text_list;
		{
			TextStruct text_struct{};
			std::vector<std::wstring> text_list_tmp;
			wifText.ReadAllLine(text_list_tmp);
			for (auto& line : text_list_tmp)
			{
				if (line.find(L"Raw:") == 0)
				{
					text_struct.m_wsRaw = line.substr(4);
					continue;;
				}

				if (line.find(L"Tra:") == 0)
				{
					text_struct.m_wsTra = line.substr(4);
					text_list.emplace_back(text_struct);
					continue;
				}
			}
		}

		RxJson::Parser parser;
		parser.Open(json_path);
		RxJson::Value json;
		parser.Read(json);
		RxJson::JArray& arrx = json[L"Scenario"].ToAry();

		size_t count = 0;
		for (auto& obj : arrx)
		{
			std::wstring& tra_txt = obj[L"Text_Tra"];
			std::wstring& raw_txt = obj[L"Text_Raw"];
			if (raw_txt == L"…―─" || raw_txt == L"。、」』）！？”～ー♪")
			{
				continue;
			}

			std::wstring& cur_txt = text_list[count].m_wsRaw;
			if (raw_txt == cur_txt)
			{
				tra_txt = text_list[count].m_wsTra;
			}
			else
			{
				RxConsole::PutFormat(L"%s\n%s\n%s\nError\n", json_path.c_str(), raw_txt.c_str(), cur_txt.c_str());
				return;
			}

			count++;
		}

		parser.Save(json, wsOutJsonPath + json_file);
	}
}

#include <Windows.h>
void MergeHsTxt(std::wstring wsTextsPath)
{
	std::unordered_map<std::wstring, std::wstring> merge_list;

	merge_list[L"scene_ak_h01.ks.txt"] = L"sn3020.ks.txt";
	merge_list[L"scene_ak_h02.ks.txt"] = L"sn3040.ks.txt";
	merge_list[L"scene_ak_h03.ks.txt"] = L"sn3060.ks.txt";
	merge_list[L"scene_hr_h01.ks.txt"] = L"sn1230.ks.txt";
	merge_list[L"scene_nt_h01.ks.txt"] = L"sn1040.ks.txt";
	merge_list[L"scene_yr_h01.ks.txt"] = L"sn2010.ks.txt";
	merge_list[L"scene_yr_h02.ks.txt"] = L"sn2030.ks.txt";
	merge_list[L"scene_yr_h03.ks.txt"] = L"sn2050.ks.txt";

	for (auto& pairx : merge_list)
	{
		std::vector<std::wstring> text_list_tmp;

		RxFile::Text wifText1{ wsTextsPath + pairx.second, RIO_READ, RFM_UTF8 };
		RxFile::Text wifText2{ wsTextsPath + pairx.first, RIO_READ, RFM_UTF8 };
		wifText1.ReadAllLine(text_list_tmp);
		wifText2.ReadAllLine(text_list_tmp);
		wifText1.Close();
		wifText2.Close();
		::DeleteFileW((wsTextsPath + pairx.first).c_str());
		::DeleteFileW((wsTextsPath + pairx.second).c_str());
		RxFile::Text ofs{ wsTextsPath + pairx.second, RIO_WRITE, RFM_UTF8 };
		ofs.WriteAllLine(text_list_tmp);
	}


}


// for import jp text
bool GetTable(std::wstring wsKS, std::vector<std::pair<std::wstring, std::wstring>>& vecText)
{
	RxFile::Text wifText{ wsKS, RIO_READ, RFM_UTF8 };

	std::vector<std::wstring> text_list;
	wifText.ReadAllLine(text_list);

	for (auto ite = text_list.begin(); ite != text_list.end(); ite++)
	{
		std::wstring& line = *ite;
		if (line.find(L"; \"") == 0)
		{
			std::wstring text = GetMsg(line);
			ite++;
			line = *ite;
			std::wstring index = GetParameterValue(line, L"mes_no=");
			vecText.emplace_back(std::make_pair(index, text));
		}
	}

	return true;
}

bool RepTable(std::wstring wsTJS, std::wstring wsSaveTJS, std::vector<std::pair<std::wstring, std::wstring>>& vecText)
{
	RxFile::Text wifText{ wsTJS, RIO_READ, RFM_UTF8 };

	std::vector<std::wstring> text_list;
	wifText.ReadAllLine(text_list);

	if (
		text_list[0].find(L"tf.nowMessageLang") != 0 ||
		text_list[1].find(L"tf.langScenario") != 0 ||
		text_list[text_list.size() - 1].find(L"];") != 0

		)
	{
		throw std::runtime_error("Error!");
	}

	// make map
	std::vector<std::wstring> index_list;
	for (auto ite = text_list.begin(); ite != text_list.end(); ite++)
	{
		std::wstring& line = *ite;
		if (line.find(L"\t") == 0)
		{
			size_t pos = line.find(L"\" => \"");
			if (pos == std::wstring::npos) { throw std::runtime_error("Error!"); }

			std::wstring index = line.substr(2, pos - 2);
			index_list.emplace_back(std::move(index));
		}
	}

	if (index_list.size() != vecText.size())
	{
		throw std::runtime_error("Error!");
	}

	size_t cnt = 0;
	for (auto& pair : vecText)
	{
		if (pair.first != index_list[cnt])
		{
			throw std::runtime_error("Error!");
		}
		cnt++;
	}

	std::wstring tjs;
	tjs.append(text_list[0]);
	tjs.append(1, L'\n');
	tjs.append(text_list[1]);
	tjs.append(1, L'\n');

	for (auto& pair : vecText)
	{
		tjs.append(1, L'\t');
		tjs.append(1, L'\"');
		tjs.append(pair.first);
		tjs.append(L"\" => \"");
		tjs.append(pair.second);
		tjs.append(L"\",\n");
	}

	tjs.append(L"];\n");

	RxFile::Text ofs{ wsSaveTJS, RIO_WRITE, RFM_UTF8 };
	ofs.WriteLine(tjs);

	return true;
}

int main()
{
	// Extraction of cross-referenced texts
	// Raw:贋壓の頼莎さと揖じように、泳溺の黛悶もきれいで´´。
	// Tra:She¨s basically a perfect being, like some kind of god, and so she¨s got the bod to match.
	// ExtractTexts(L"ks/lang_en/", L"ks/main/", L"txt/");
	//ExtractTexts(L"ks/lang_zhcn/", L"ks/main/", L"txt/");
	//ExtractTexts(L"ks/lang_zhtw/", L"ks/main/", L"txt/");

	//MergeHsTxt(L"txt/ks/lang_zhcn/");
	//MergeHsTxt(L"txt/ks/lang_en/");
	//MergeHsTxt(L"txt/ks/lang_zhtw/");

	//ImportToJson(L"json/jp/", L"txt/ks/lang_en/", L"json/en/");
	//ImportToJson(L"json/jp/", L"txt/ks/lang_zhtw/", L"json/zhtw/");

	std::vector<std::wstring> ks_list;
	RxPath::CurFileNames(L"ks/", ks_list);

	for (auto& ks:ks_list)
	{
		std::vector<std::pair<std::wstring, std::wstring>> jp_table;
		GetTable(L"ks/" + ks, jp_table);
		std::wstring tjs_name = ks.substr(0, ks.size() - 3) + L"_lang.tjs";
		RepTable(L"tjs/" + tjs_name, L"tjs_out/" + tjs_name, jp_table);
	}


	int a = 0;
}