#include "funnelhash.h"

#include <iostream>

using MyFunnelHash = FunnelHash<std::string_view, std::string_view, 8>;

void fetch(MyFunnelHash const &h, std::string_view k){
	std::cout << "Fetch " << k << " -> "
			<< h.get   (k, "--none--") << " "
			<< h.get<1>(k, "--none--") << '\n';
}

int main(){
	MyFunnelHash h;

	h.push("Name",		"Peter"		);
	h.push("Age",		"22"		);
	h.push("City",		"Sofia"		);
	h.push("Country",	"BG"		);
	h.push("Currency",	"BGN"		);
	h.push("PC",		"Dell"		);
	h.push("Model",		"XPS"		);
	h.push("Screen",	"15'"		);
	h.push("CPU",		"Intel"		);
	h.push("CPU Model",	"i7-12700H"	);
	h.push("Cores",		"6"		);
	h.push("ECores",	"8"		);
	h.push("Threads",	"20"		);
	h.push("GPU",		"NVDA"		);
	h.push("HDD",		"1 TB NVME"	);
	h.push("Mouse",		"Xiaomy"	);
	h.push("Mouse",		"Logitech"	);

	h.remove("GPU");

	h.print();

	fetch(h, "Cores"	);
	fetch(h, "GPU"		);
	fetch(h, "Mouse"	);
	fetch(h, "HDD"		);
}

