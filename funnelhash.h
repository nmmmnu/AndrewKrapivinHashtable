#include "murmur_hash_64a.h"

#include <array>
#include <vector>
#include <iostream>

template<typename K, typename V, size_t N>
struct FunnelHash{
	size_t const TRIES = 3;

	enum class PushResult{
		ERROR		,
		INSERTED	,
		UPDATED
	};

	void clear(){
		storage_.clear();
	}

	template<typename UV>
	PushResult push(K const &k, UV &&v){
		size_t level = 0;

		while(true){
			if (level == storage_.size()){
				// create level
				storage_.push_back({});
			}

			auto &storage = storage_[level];

			for(size_t tries = 0; tries < TRIES; ++tries){
				auto const cell = hash__(k, tries);

				if (storage[cell].empty()){
					// insert here
					storage[cell].key = k;
					storage[cell].val = std::forward<UV>(v);

					return PushResult::INSERTED;
				}

				if (storage[cell].key == k){
					// already inserted, update
					storage[cell].val = std::forward<UV>(v);

					return PushResult::UPDATED;
				}
			}

			++level;
		}

	//	return PushResult::ERROR;
	}

	auto const &operator()(K const &k, V const &defv) const{
	//	std::cout << "Key : " << k << '\n';

		size_t level = 0;

		while(true){
			if (level == storage_.size()){
				// no such level
				return defv;
			}

			auto &storage = storage_[level];

			for(size_t tries = 0; tries < TRIES; ++tries){
				auto const cell = hash__(k, tries);

			//	std::cout << "Probing : " << level << " " << cell << " " << storage[cell].key << '\n';

				if (storage[cell].empty()){
					// not found
					return defv;
				}

				if (storage[cell].key == k){
					// found
					return storage[cell].val;
				}
			}

			++level;
		}
	}

	void print() const{
		for(auto &a : storage_){
			std::cout << "Level: {" << '\n';
			std::cout << '\t';
			for(auto &[k,v] : a){
				if (empty(k))
					std::cout << " {}, ";
				else
					std::cout << " { " << k << " : " << v << " }, ";
			}
			std::cout << '\n' << "}" << '\n';
		}
	}

private:
	// fix for non std::string_views
	static auto hash__(K const &k, size_t seed){
		return murmur_hash64a(k, seed) % N;
	}

	// fix for non std::string_views
	constexpr static bool empty__(K const &k){
		return k.empty();
	}

private:
	struct Pair{
		K key;
		V val;

		constexpr bool empty() const{
			return empty__(key);
		}
	};

	using Array = std::array<Pair, N>;

private:
	std::vector<Array> storage_;
};

