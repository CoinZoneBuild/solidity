/*
	This file is part of solidity.

	solidity is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	solidity is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with solidity.  If not, see <http://www.gnu.org/licenses/>.
*/
/**
 * @author Alex Beregszaszi
 * @date 2016
 * Standard JSON compiler interface.
 */

#pragma once

#include <libsolidity/interface/CompilerStack.h>

namespace dev
{

namespace solidity
{

/**
 * Standard JSON compiler interface, which expects a JSON input and returns a JSON ouput.
 * See docs/using-the-compiler#compiler-input-and-output-json-description.
 */
class StandardCompiler: boost::noncopyable
{
public:
	struct ReadFileResult
	{
		bool success;
		std::string contentsOrErrorMessage;
	};

	/// File reading callback.
	using ReadFileCallback = std::function<ReadFileResult(std::string const&)>;

	/// Creates a new StandardCompiler.
	/// @param _readFile callback to used to read files for import statements. Should return
	StandardCompiler(ReadFileCallback const& _readFile = ReadFileCallback())
	{
		CompilerStack::ReadFileCallback fileReader = [this,_readFile](std::string const& _path)
		{
			auto ret = _readFile(_path);
			return CompilerStack::ReadFileResult{ret.success, ret.contentsOrErrorMessage};
		};

		m_compilerStack.reset(new CompilerStack(fileReader));
	}

	/// Sets all input parameters according to @a _input which conforms to the standardized input
	/// format.
	Json::Value compile(Json::Value const& _input);
	std::string compile(std::string const& _input);

private:
	CompilerStack m_compilerStack;
};

}
}
