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

	h.add("Name",		"Peter"		);
	h.add("Age",		"22"		);
	h.add("City",		"Sofia"		);
	h.add("Country",	"BG"		);
	h.add("Currency",	"BGN"		);
	h.add("PC",		"Dell"		);
	h.add("Model",		"XPS"		);
	h.add("Screen",		"15'"		);
	h.add("CPU",		"Intel"		);
	h.add("CPU Model",	"i7-12700H"	);
	h.add("Cores",		"6"		);
	h.add("ECores",		"8"		);
	h.add("Threads",	"20"		);
	h.add("GPU",		"NVDA"		);
	h.add("HDD",		"1 TB NVME"	);
	h.add("Mouse",		"Xiaomy"	);
	h.add("Mouse",		"Logitech"	);

	h.remove("GPU");

	h.print();

	fetch(h, "Cores"	);
	fetch(h, "GPU"		);
	fetch(h, "Mouse"	);
	fetch(h, "HDD"		);
}

