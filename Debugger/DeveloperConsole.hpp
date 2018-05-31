#ifndef DEVELOPER_CONSOLE_H
#define DEVELOPER_CONSOLE_H
#include <string>

#undef new
#include <map>
//#define new new( __FILE__, __LINE__ )

#include "RegisteredCommand.hpp"
#include "../Render/BitmapFont.hpp"
#include "../Utilities/RGBColor.hpp"
#include "../Utilities/Vector3.h"

const Vector3 START_POS = Vector3( 18.f, 0.f, 0.f );

struct Command
{
	std::string message;
	RGBColor color;
};

class DeveloperConsole
{
	public:
		std::map<std::string, RegisteredCommand*> m_registeredCommandList;
		bool m_isOpen;
		BitmapFont* m_font;
		std::string m_stringToPrintOnConsolePrompt;
		std::vector<Command> m_historyCommandListToPrint;
		std::vector<Command> m_historyCommandList;

	private:
		std::string m_stringToPrintOnScreen;
		RGBColor m_colorToPrintOnScreen;
		Vector2 m_cursorPos;
		float m_moveCursor;
		int m_cursorPosInString;
		float m_scrollOffset;
		bool m_isScrollAble;
		float m_currentHeight;
		float m_currentBottom;

	public:
		void InitializeConsole();
		void RegisterConsoleCommand(const std::string& commandName, CommandFuncPtr functionPtr, const std::string& desc);
		bool ExecuteCommand(const std::string& commandName, const std::string& args);
		bool ParseMessageAndExecuteCommand();
		void ProcessKeyboard( unsigned char keyboard );
		void Render();
		void RenderCursor();
		void ProcessSpecialKeyboard( unsigned char key );
		void ParseArgument(const std::string& args, std::vector<std::string>& argComponent);

	private:
		void ParseMessage();
};

void QuitCommand( const std::string& args );
void HelpCommand( const std::string& args );
void ClearCommand( const std::string& args );
void TestCommand( const std::string& args );
#endif