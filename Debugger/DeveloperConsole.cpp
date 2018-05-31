#include "DeveloperConsole.hpp"
#include "../Utilities/GameCommon.hpp"
#include "../Utilities/ProfileSection.hpp"
#include "../Render/GraphicManager.hpp"
#include "../JobSystem/MemoryPoolManager.hpp"

void DeveloperConsole::InitializeConsole()
{
	m_isOpen = false;
	m_font = new BitmapFont("./Data/Font/MainFont_EN.FontDef.xml","./Data/Font/MainFont_EN00.png");
	m_moveCursor = 0.f;
	m_cursorPosInString = 0;
	m_scrollOffset = 0.f;
	m_isScrollAble = false;
	m_currentHeight = 0.f;
	m_currentBottom = 18.f;
	RegisterConsoleCommand("quit", QuitCommand, "Quit Program.");
	RegisterConsoleCommand("help", HelpCommand, "Display Command List.");
	RegisterConsoleCommand("clear", ClearCommand, "Clear Screen.");
	RegisterConsoleCommand("test", TestCommand, "Test Command With Arguments.");
	RegisterConsoleCommand( "profiling", ReportProfilingResultCommand, "Report Profiling Results." );
}

void DeveloperConsole::RegisterConsoleCommand(const std::string& commandName, CommandFuncPtr functionPtr, const std::string& desc)
{
	RegisteredCommand* registeredCommand = new RegisteredCommand();
	registeredCommand->m_callbackFunction = functionPtr;
	registeredCommand->m_commandName = commandName;
	registeredCommand->m_commandDescription = desc;
	m_registeredCommandList[commandName] = registeredCommand;
}

bool DeveloperConsole::ExecuteCommand( const std::string& commandName, const std::string& args )
{
	std::map<std::string, RegisteredCommand*>::iterator it;
	it = m_registeredCommandList.find(commandName);
	if( it == m_registeredCommandList.end() )
	{
		DebuggerPrintf("Cannot find command 123!\n");
		return false;
	}
	else
	{
		it->second->m_callbackFunction(args);
		return true;
	}
}

void DeveloperConsole::Render()
{
	GraphicManager::s_render->DisableAllTextureUnit();
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);

	glPushMatrix();
	glLoadIdentity();
	glOrtho( 0.0, WINDOW_PHYSICAL_WIDTH, 0.0, WINDOW_PHYSICAL_HEIGHT ,0.f, 1.0f );

	glColor4f( 0.f, 0.f, 0.f, 1.0f );
	glBegin(GL_QUADS);
	{
		//draw console window
		glVertex2f(-10.f,-10.f);
		glVertex2f(1600.f,-10.f);
		glVertex2f(1600.f,40.f);
		glVertex2f(-10.f,40.f);
	}
	glEnd();

	glLineWidth( 3.0 );
	glEnable( GL_BLEND );
	glColor4f( 0.f, 0.f, 0.f, 0.8f );
	glBegin(GL_QUADS);
	{
		//draw translucent quads to cover screen
		glVertex2f( -10.f, -10.f);
		glVertex2f( 1600.f,-10.f);
		glVertex2f( 1600.f, 900.f);
		glVertex2f( -10.f, 900.f);
	}
	glEnd();

	glColor4f( 1.0f, 0.7f, 0.8f, 1.f );
	glBegin(GL_LINES);
	{
		//draw console line
		glVertex2f( 1600.f, 40.f );
		glVertex2f( -10.f, 40.f );
	}
	glEnd();

	m_historyCommandList = m_historyCommandListToPrint;

	if( m_historyCommandListToPrint.size() > 0 )
	{
		float startPosY = 10.f;
		int offset = 0;
		for( int i = m_historyCommandListToPrint.size() - 1; i >= 0 ; i--)
		{
			offset++;
			Vector3 position = Vector3( 18.f, startPosY + offset * 40.0f + m_scrollOffset, 0.f );
			if( position.y > 30.0f )
				GraphicManager::s_render->RenderText( position, Vector4( 0.f, 0.f, 0.f, 0.f ), Vector3( 1.f, 1.f, 1.f ), m_historyCommandListToPrint[i].color, nullptr, nullptr, 25, nullptr, m_historyCommandListToPrint[i].message);
			m_currentHeight = offset * 40.0f + m_scrollOffset;
		}
		if(m_currentHeight > 800.f)
		{
			m_isScrollAble = true;
		}
	}
 	
	if( m_stringToPrintOnConsolePrompt.size() > 0 )
	{
		GraphicManager::s_render->RenderText( START_POS, Vector4( 0.f, 0.f, 0.f, 0.f ), Vector3( 1.f, 1.f, 1.f ), RGBColor(0.f,1.f,0.f,1.f), nullptr, nullptr, 30, nullptr, m_stringToPrintOnConsolePrompt);
	}

	RenderCursor();

	glPopMatrix();
	glEnable(GL_DEPTH_TEST);
	glColor4f( 1.f, 1.f, 1.f, 1.f );
	glLineWidth( 1.0 );
}

void DeveloperConsole::ProcessKeyboard( unsigned char key )
{
	if( key == VK_ESCAPE && m_stringToPrintOnConsolePrompt.size() > 0 )
	{
		m_stringToPrintOnConsolePrompt.clear();
		m_cursorPosInString = 0;
		m_moveCursor = 0.f;
	}
	else if( key == VK_ESCAPE && m_stringToPrintOnConsolePrompt.size() <= 0)
	{
		m_isOpen = false;
	}
	else if( key == VK_BACK && m_stringToPrintOnConsolePrompt.size() > 0)
	{
		if( m_cursorPosInString > 0 )
		{
			std::string temp;
			temp.append(1, m_stringToPrintOnConsolePrompt[m_cursorPosInString - 1] );
			m_moveCursor -= GraphicManager::s_render->CalcTextWidth(nullptr,30,temp);
			m_stringToPrintOnConsolePrompt.erase( m_stringToPrintOnConsolePrompt.begin() + m_cursorPosInString - 1 );
			m_cursorPosInString--;
		}
		else if( m_cursorPosInString == 0 )
		{
			m_stringToPrintOnConsolePrompt.erase( m_stringToPrintOnConsolePrompt.begin() );
			m_moveCursor = 0;
		}
	}
	else if( key == VK_BACK && m_stringToPrintOnConsolePrompt.size() <= 0 )
	{
		m_cursorPosInString = 0;
		m_moveCursor = 0.f;
	}
	else if( key == VK_RETURN && m_stringToPrintOnConsolePrompt.size() > 0 )
	{
		if(ParseMessageAndExecuteCommand() == false)
		{
			Command temp;
			m_stringToPrintOnScreen.append("The \"");
			m_stringToPrintOnScreen.append(m_stringToPrintOnConsolePrompt);
			m_stringToPrintOnScreen.append("\"");
			m_stringToPrintOnScreen.append(" is not a valid command. Enter \"Help\" to see command list.");
			temp.message = m_stringToPrintOnScreen;
			temp.color = RGBColor(1.f,0.f,0.f,1.f);
			m_historyCommandListToPrint.push_back(temp);
		}
		m_stringToPrintOnScreen.clear();
		m_stringToPrintOnConsolePrompt.clear();
		m_cursorPosInString = 0;
		m_moveCursor = 0.f;
	}
	else if( key == VK_RETURN && m_stringToPrintOnConsolePrompt.size() <= 0 )
	{
		m_stringToPrintOnConsolePrompt.clear();
		m_cursorPosInString = 0;
		m_moveCursor = 0.f;
	}
	else if( key == VK_TAB )
	{

	}
	else if( key != 96 )
	{
		std::string temp;
		m_cursorPosInString++;
		temp.append( 1, key );
		m_stringToPrintOnConsolePrompt.insert(m_cursorPosInString - 1,temp);
		temp.clear();
		temp.append(1, m_stringToPrintOnConsolePrompt[m_cursorPosInString - 1] );
		m_moveCursor += GraphicManager::s_render->CalcTextWidth(nullptr,30,temp);
	}
}

bool DeveloperConsole::ParseMessageAndExecuteCommand()
{
	std::string messageTemp = m_stringToPrintOnConsolePrompt;
	for(unsigned int i = 0; i < messageTemp.size(); i++)
	{
		char temp;
		temp = messageTemp[i];
		temp = static_cast<char>( tolower(temp) );
		messageTemp[i] = temp;
	}
	std::string command;
	std::string args;
	int spacePos = messageTemp.find(" ");
	if(spacePos != -1)
	{
		command = messageTemp.substr(0, spacePos);
		args = messageTemp.substr(spacePos + 1);
	}
	else
		command = messageTemp;

	if(ExecuteCommand( command, args ))
		return true;
	else 
		return false;
}

void DeveloperConsole::ProcessSpecialKeyboard(unsigned char key)
{
	if( key == VK_LEFT && m_stringToPrintOnConsolePrompt.size() > 0)
	{
		if(m_cursorPosInString > 0)
		{
			m_cursorPosInString--;
			std::string temp;
			temp.append(1, m_stringToPrintOnConsolePrompt[m_cursorPosInString] );
			m_moveCursor -= GraphicManager::s_render->CalcTextWidth(nullptr,30,temp);
		}
		else if(m_cursorPosInString == 0)
		{
			m_cursorPosInString = 0;
			m_moveCursor = 0.f;
		}
	}
	else if( key == VK_RIGHT && m_stringToPrintOnConsolePrompt.size() > 0)
	{
		if( m_cursorPosInString < static_cast<int>( m_stringToPrintOnConsolePrompt.size() ) && static_cast<int>( m_stringToPrintOnConsolePrompt.size() ) > 0)
		{
			m_cursorPosInString++;
			std::string temp;
			temp.append(1, m_stringToPrintOnConsolePrompt[m_cursorPosInString - 1] );
			m_moveCursor += GraphicManager::s_render->CalcTextWidth(nullptr,30,temp);
		}
		else if(m_cursorPosInString == static_cast<int>( m_stringToPrintOnConsolePrompt.size() ) )
		{
			m_moveCursor = GraphicManager::s_render->CalcTextWidth(nullptr,30,m_stringToPrintOnConsolePrompt);
		}
	}
	else if( key == VK_NEXT && m_isScrollAble )
	{
		if( m_currentHeight > 800.f )
		{
			m_scrollOffset -= 10.f;
			m_currentBottom += 10.f;
		}
	}
	else if( key == VK_PRIOR  && m_currentBottom > 18)
	{
		m_scrollOffset += 10.f;
		m_currentBottom -= 10.f;
	}
	else if( key == VK_HOME)
	{
		m_cursorPosInString = 0;
		m_moveCursor = 0;
	}
	else if( key == VK_END)
	{
		m_cursorPosInString = m_stringToPrintOnConsolePrompt.size();
		m_moveCursor = GraphicManager::s_render->CalcTextWidth(nullptr,30,m_stringToPrintOnConsolePrompt);
	}
	else if( key == VK_UP && m_historyCommandList.size() > 0 )
	{
	}
}

void DeveloperConsole::ParseArgument(const std::string& args, std::vector<std::string>& argComponents)
{
	std::string argTemp = args;
	argTemp.append(" ");
	for( unsigned int i = 0; i < args.size(); i++)
	{
		int blank = argTemp.find(" ");
		std::string component = argTemp.substr(0,blank);
		argTemp.erase(0, blank + 1);
		if(component != "")
			argComponents.push_back(component);
	}
}

void DeveloperConsole::RenderCursor()
{
	static float num  = 0;
	num += 0.05f;

	glLineWidth( 2.f );
	glDisable(GL_TEXTURE_2D);
	glBegin( GL_LINES );
	glColor4f( 1.f - 0.5f * sin(num), 1.f - 0.5f * sin(num), 1.f - 0.5f * sin(num), 1.f );
	glVertex2f( START_POS.x + m_moveCursor, 5.f );
	glVertex2f( START_POS.x + m_moveCursor, 30.f);
	glEnd();
}

void QuitCommand(const std::string&)
{
	exit(0);
}

void HelpCommand(const std::string&)
{
	std::map<std::string, RegisteredCommand*>::iterator it;
	for(it = g_theConsole->m_registeredCommandList.begin(); it != g_theConsole->m_registeredCommandList.end(); ++it )
	{
		std::string temp;
		Command temp1;
		temp.append( " - ");
		temp.append( it->second->m_commandName );
		temp.append( " : ");
		temp.append( it->second->m_commandDescription );
		temp1.message = temp;
		temp1.color = RGBColor(1.f,1.f,1.f,1.f);
		g_theConsole->m_historyCommandListToPrint.push_back(temp1);
	}
}

void ClearCommand(const std::string&)
{
	g_theConsole->m_historyCommandListToPrint.clear();
}

void TestCommand(const std::string& args)
{
	std::vector<std::string> argComponents;
	g_theConsole->ParseArgument(args, argComponents);
	Command temp1;
	std::string temp;
	temp1.color = RGBColor(1.f,1.f,1.f,1.f);

	if(argComponents.size() == 0)
	{
		temp = "Execute \"Test\" with no argument.";
	}
	else if(argComponents.size() == 1)
	{
		if(argComponents[0] == "1")
		{
			temp = "Execute \"Test\" with argument \"1\".";
		}
		else if(argComponents[0] == "2")
		{
			temp1.color = RGBColor(1.f,1.f,1.f,1.f);
			temp = "Execute \"Test\" with argument \"2\".";
		}
	}
	else if(argComponents.size() == 2)
	{
		temp = "Execute \"Test\" with arguments ";
		temp.append("\"");
		temp.append(argComponents[0]);
		temp.append("\"");
		temp.append(" and ");
		temp.append("\"");
		temp.append(argComponents[1]);
		temp.append("\".");
	}
	else
	{
		temp1.color = RGBColor(1.f,0.f,0.f,1.f);
		temp = "\"Test\" does not take more than 2 arguments.";
	}
	temp1.message = temp;
	g_theConsole->m_historyCommandListToPrint.push_back(temp1);
}