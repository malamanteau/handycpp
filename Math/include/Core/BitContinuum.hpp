
/// See ../../License.txt for license info.

#pragma once

#include "Base.hpp"
#include "Popcount.hpp"

namespace HANDYMATH_NS {

	/// Represents an infinite bitfield. Implemented as a vector of blocks.
	/// While although it represents an infinite bitfield, the number of bytes needed for memory
	/// is the following: ceil[(highest bit set) / 64] * sizeof(uint64_t) + sizeof(bool)
	/// 
	/// This data structure will only grow with time and is DENSE
	/// 
	/// RepeatingBit is the endless value used in the continuum:
	///     RepeatingBit == 0 means that a default BitContinuum is 0000000000000..... 
	///         ...and .all()  is ALWAYS false                           
	///     RepeatingBit == 1 means that a default BitContinuum is 1111111111111..... 
	///         ...and .none()  is ALWAYS false 
	///         ...and .any()   is ALWAYS true
	///         ...and .count() is ALWAYS the max for uint64_t 
	///
	/// Effect of bitwise operations on RepeatingBit:
	///       LHS    RHS    Result
	/// 
	/// not    1              0
	///        0              1
	/// and    0      0       0 
	///        0      1       0
	///        1      0       0
	///        1      1       1
	/// or     0      0       0
	///        0      1       1
	///        1      0       1
	///        1      1       1
	///
	/// Note that on 32-bit builds you will be limited to a top index of (64 * 4294967296), but at 
	/// that point you'll be using 4 GiB of memory for a single continuum. The compiler will probably
	/// complain about truncation casting as well.
	/// On 64-bit builds you need not worry, you'll run out of memory looong before you hit the top
	/// index (this is a dense implemenation after all).
	class BitContinuum
	{
		static constexpr uint64_t BlockSize = sizeof(uint64_t) * 8;

		uint64_t fillBlockValue() const;
		bool ensure_min_blocks(uint64_t numBlocks);
		bool ensure_min_bits  (uint64_t numBits);

	public:
		COPY_CTOR(BitContinuum, default)
		ASSIGN_CTOR(BitContinuum, default)

		BitContinuum(BitContinuum &&); 
		BitContinuum & operator=(BitContinuum &&);

		std::vector<uint64_t> Blocks;
		bool                  RepeatingBit;

		explicit BitContinuum(bool repeatingBit = false);

		uint64_t size_bits() const;

		bool get_bit(uint64_t reqBitIndex) const;

		/// IN-PLACE!
		
		BitContinuum & set_bit(uint64_t reqBitIndex, bool value = true);
		BitContinuum & bit_not();
		BitContinuum & bit_and(BitContinuum const & bb);
		BitContinuum & bit_or (BitContinuum const & bb);

		bool  all() const;
		bool none() const;
		bool  any() const;

		uint64_t count() const;

		bool operator[](uint64_t index);

		/// Iterate the indexes of bits set (if RepeatingBit is 0)
		/// OR bits UNSET (if RepeatingBit is 1)
		class iterator
		{
		public:
			uint64_t bit_index() const;
			bool     step();

			friend class BitContinuum;
		private:
			uint64_t m_bitIndex   = std::numeric_limits<uint64_t>::max();
			uint64_t m_blockIndex = 0;
			uint64_t m_mask       = 0;

			BitContinuum & m_ref;

			iterator(BitContinuum & ref);
		};

		iterator get_iterator();
	};

	FORCEINLINE BitContinuum::BitContinuum(BitContinuum && other)
	{
		*this = std::move(other);
	}

	FORCEINLINE BitContinuum & BitContinuum::operator=(BitContinuum && other)
	{
		if (this != &other)
		{
			Blocks = std::move(other.Blocks);
			RepeatingBit = other.RepeatingBit;
		}

		return *this;
	}

	FORCEINLINE uint64_t BitContinuum::fillBlockValue() const { return RepeatingBit ? (~0_u64) : 0_u64; }

	FORCEINLINE bool BitContinuum::ensure_min_blocks(uint64_t numBlocks)
	{
		if (Blocks.size() < numBlocks)
		{
			Blocks.resize(numBlocks, fillBlockValue());
			return true;
		}
		return false;
	}

	FORCEINLINE bool BitContinuum::ensure_min_bits(uint64_t numBits)
	{
		uint64_t numBlocks = numBits / BlockSize + (numBits % BlockSize ? 1 : 0);
		return ensure_min_blocks(numBlocks);
	}

	FORCEINLINE BitContinuum::BitContinuum(bool repeatingBit/* = false*/) : RepeatingBit(repeatingBit) { }

	FORCEINLINE uint64_t BitContinuum::size_bits() const { return Blocks.size() * BlockSize; }
	FORCEINLINE bool     BitContinuum::get_bit(uint64_t reqBitIndex) const
	{
		uint64_t blockIndex = reqBitIndex / BlockSize;
		uint64_t bitIndex   = reqBitIndex % BlockSize;

		if (blockIndex >= Blocks.size())
			return RepeatingBit;

		uint64_t block = Blocks[blockIndex];
		return (block >> bitIndex) & (1_u64);
	}


	FORCEINLINE BitContinuum & BitContinuum::set_bit(uint64_t reqBitIndex, bool value/* = true*/)
	{
		ensure_min_bits(reqBitIndex + 1);

		uint64_t blockIndex = reqBitIndex / BlockSize;
		uint64_t bitIndex   = reqBitIndex % BlockSize;

		if (value)
			Blocks[blockIndex] |= (1_u64 << bitIndex);
		else
			Blocks[blockIndex] &= (~(1_u64 << bitIndex));

		return *this;
	}

	FORCEINLINE BitContinuum & BitContinuum::bit_not()
	{
		for (auto & block : Blocks)
			block = ~block;

		RepeatingBit = !RepeatingBit;

		return *this;
	}

	FORCEINLINE BitContinuum & BitContinuum::bit_and(BitContinuum const & bb)
	{
		ensure_min_blocks(bb.Blocks.size());

		for (uint64_t blockI = 0; blockI < bb.Blocks.size(); ++blockI)
		{
			Blocks[blockI] &= bb.Blocks[blockI];
		}

		RepeatingBit = RepeatingBit && bb.RepeatingBit;

		return *this;
	}

	FORCEINLINE BitContinuum & BitContinuum::bit_or(BitContinuum const & bb)
	{
		ensure_min_blocks(bb.Blocks.size());

		for (uint64_t blockI = 0; blockI < bb.Blocks.size(); ++blockI)
		{
			Blocks[blockI] |= bb.Blocks[blockI];
		}

		RepeatingBit = RepeatingBit || bb.RepeatingBit;

		return *this;
	}

	FORCEINLINE bool BitContinuum::all() const
	{
		if (!RepeatingBit)
			return false;

		for (auto & block : Blocks)
			if (~block)
				return false;
			
		return true;
	}

	FORCEINLINE bool BitContinuum::none() const
	{
		if (RepeatingBit)
			return false;

		for (auto & block : Blocks)
			if (block)
				return false;
		
		return true;
	}

	FORCEINLINE bool BitContinuum::any() const
	{
		if (RepeatingBit)
			return true;

		for (auto & block : Blocks)
			if (block)
				return true;
			
		return false;
	}

	FORCEINLINE uint64_t BitContinuum::count() const
	{
		if (RepeatingBit)
			return std::numeric_limits<uint64_t>::max();

		int64_t num = 0;

		for (auto & block : Blocks)
			num += HANDYMATH_NS::Popcount(block);

		return (uint64_t)num;
	}

	FORCEINLINE bool BitContinuum::operator[](uint64_t index) { return get_bit(index); } 

	FORCEINLINE BitContinuum::iterator BitContinuum::get_iterator() { return iterator(*this); }

	FORCEINLINE BitContinuum::iterator::iterator(BitContinuum & ref) : m_ref(ref) { }

	FORCEINLINE uint64_t BitContinuum::iterator::bit_index() const { return m_bitIndex; }

	FORCEINLINE bool BitContinuum::iterator::step()
	{
		uint64_t block = 0;

		while (true)
		{
			if (m_blockIndex == m_ref.Blocks.size())
				return false;

			block = m_ref.Blocks[m_blockIndex];

			if (m_ref.RepeatingBit)
				block = ~block;

			block &= (~m_mask);

			if (block)
				break;

			++m_blockIndex;
			m_mask = 0;

		} 

		uint8_t bitblockindex = HANDY_NS::BitscanLSB(block);
		m_bitIndex = BlockSize * m_blockIndex + bitblockindex;
		m_mask = ((~0_u64) >> (63_u8 - bitblockindex));

		return true;
	}
}
