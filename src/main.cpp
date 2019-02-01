/*!
 *  \file    main.cpp
 *  \author  Caleb Amoa Buahin <caleb.buahin@gmail.com>
 *  \version 1.0.0
 *  \section Description
 *  This file and its associated files and libraries are free software;
 *  you can redistribute it and/or modify it under the terms of the
 *  Lesser GNU Lesser General Public License as published by the Free Software Foundation;
 *  either version 3 of the License, or (at your option) any later version.
 *  fvhmcompopnent.h its associated files is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.(see <http://www.gnu.org/licenses/> for details)
 *  \date 2018
 *  \pre
 *  \bug
 *  \todo
 *  \warning
 */

#include "stdafx.h"
// #include "elementjunction.h"
// #include "cshmodel.h"
// #include "test/cshcomponenttest.h"

#include <QCoreApplication>
#include <QCommandLineParser>

void initializeCommandLineParser(QCommandLineParser &commandLineParser)
{
//   commandLineParser.setApplicationDescription("CSHComponent");
//   commandLineParser.setSingleDashWordOptionMode(QCommandLineParser::ParseAsLongOptions);
//   commandLineParser.addHelpOption();
//   commandLineParser.addVersionOption();

//   commandLineParser.addPositionalArgument("file","The composition file to open/run.");

//   const QCommandLineOption runOption({"r","run","execute"},"Execute action. Executes the composition <file> specified to completion.","file");
//   commandLineParser.addOption(runOption);

//   const QCommandLineOption runTest({"t","test"},"Execute tests");
//   commandLineParser.addOption(runTest);
}

int main(int argc, char** argv)
{

//   qputenv("QTEST_FUNCTION_TIMEOUT", "10000000");

//   QCoreApplication *application = new QCoreApplication(argc, argv);
//   application->setOrganizationName("hydrocouple");
//   application->setOrganizationDomain("hydrocouple.org");
//   application->setApplicationName("CSHComponent");
//   application->setApplicationVersion("1.0.0");

//   QCommandLineParser commandLineParser;
//   initializeCommandLineParser(commandLineParser);
//   commandLineParser.parse(application->arguments());

//   //version
//   if(commandLineParser.isSet("version"))
//   {
//     printf("Application Name: %s\nApplication Version: %s\n", "HydroCoupleComposer",
//            qPrintable(QCoreApplication::applicationVersion()));
//   }
//   //help
//   else if (commandLineParser.isSet("help"))
//   {
//     commandLineParser.showHelp();
//   }
//   //execute specified file
//   else if(!commandLineParser.positionalArguments().isEmpty() && QFile::exists(commandLineParser.positionalArguments().first()))
//   {
//     //Error messages
//     std::list<std::string> errors;

//     //Stream temperature model instance
//     CSHModel *model = new CSHModel(nullptr);
//     model->setInputFile(QFileInfo(commandLineParser.positionalArguments().first()));

//     //initialize model
//     if(model->initialize(errors))
//     {
//       //Perform timestep until completion
//       while (model->currentDateTime() < model->endDateTime())
//       {
//         model->update();
//       }
//     }
//     else
//     {
//       printf("Errors found!\n");

//       for(std::string error : errors)
//       {
//         printf("%s\n" , error.c_str());
//       }
//     }

//     //finalize model
//     model->finalize(errors);

//     delete model;
//   }
//   else if(commandLineParser.isSet("r") && commandLineParser.value("r").size() && QFile::exists(commandLineParser.value("r")))
//   {
//     //Error messages
//     std::list<std::string> errors;

//     //Stream temperature model instance
//     CSHModel *model = new CSHModel(nullptr);
//     model->setInputFile(QFileInfo(commandLineParser.positionalArguments().first()));

//     //initialize model
//     if(model->initialize(errors))
//     {
//       //Perform timestep until completion
//       while (model->currentDateTime() < model->endDateTime())
//       {
//         model->update();
//       }
//     }
//     else
//     {
//       printf("Errors found!\n");

//       for(std::string error : errors)
//       {
//         printf("%s\n" , error.c_str());
//       }
//     }

//     //finalize model
//     model->finalize(errors);

//     delete model;
//   }
//   else
//   {

//     int status = 0;

//     //Test One
//     {

//       CSHComponentTest modelTest;
//       status |= QTest::qExec(&modelTest, argc, argv);
//     }
//   }

//   application->quit();

  return 0;
}
