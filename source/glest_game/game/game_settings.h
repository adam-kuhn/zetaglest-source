// ==============================================================
//      This file is part of Glest (www.glest.org)
//
//      Copyright (C) 2001-2008 Martiño Figueroa
//
//      You can redistribute this code and/or modify it under
//      the terms of the GNU General Public License as published
//      by the Free Software Foundation; either version 2 of the
//      License, or (at your option) any later version
// ==============================================================

#ifndef _GLEST_GAME_GAMESETTINGS_H_
#   define _GLEST_GAME_GAMESETTINGS_H_

#   include "game_constants.h"
#   include "conversion.h"
#   include <algorithm>
#   include "xml_parser.h"
#   include "config.h"
#   include "platform_common.h"
#   include "conversion.h"
#   include "platform_util.h"
#   include "leak_dumper.h"

using namespace
  Shared::Util;
using namespace
  Shared::PlatformCommon;
using namespace
  Shared::Platform;
using
  Shared::Xml::XmlNode;

namespace
  Glest
{
  namespace
    Game
  {

    enum ModType
    {
      mt_None,
      mt_Map,
      mt_Tileset,
      mt_Techtree,
      mt_Scenario
    };

// ===============================
//      class ModInfo
// ===============================

    class
      ModInfo
    {
    public:
      string
        name;
      string
        url;
      string
        imageUrl;
      string
        description;
      string
        count;                  // used for faction count for example
      string
        crc;
      string
        localCRC;
      ModType
        type;
    public:
      ModInfo ();
    };

// =====================================================
//      class GameSettings
// =====================================================

    enum FlagTypes1
    {
      ft1_none = 0x00,
      ft1_show_map_resources = 0x01,
      ft1_allow_team_switching = 0x02,
      ft1_allow_in_game_joining = 0x04,
      ft1_network_synch_checks_verbose = 0x08,
      ft1_network_synch_checks = 0x10,
      ft1_allow_shared_team_units = 0x20,
      ft1_allow_shared_team_resources = 0x40
        //ft1_xxx = 0x80
    };

    inline static bool
    isFlagType1BitEnabled (uint32 flagValue, FlagTypes1 type)
    {
      return ((flagValue & (uint32) type) == (uint32) type);
    }

    enum NetworkPlayerStatusType
    {
      npst_None = 0,
      npst_PickSettings = 1,
      npst_BeRightBack = 2,
      npst_Ready = 3,
      npst_Disconnected = 4
    };

    class
      GameSettings
    {
    private:
      string
        gameName;
      string
        description;
      string
        map;
      string
        tileset;
      string
        tech;
      string
        scenario;
      string
        scenarioDir;
      string
        factionTypeNames[GameConstants::maxPlayers];    //faction names
      string
        networkPlayerNames[GameConstants::maxPlayers];
      string
        networkPlayerPlatform[GameConstants::maxPlayers];
      int
        networkPlayerStatuses[GameConstants::maxPlayers];
      string
        networkPlayerLanguages[GameConstants::maxPlayers];
      int
        networkPlayerGameStatus[GameConstants::maxPlayers];

      ControlType
        factionControls[GameConstants::maxPlayers];
      int
        resourceMultiplierIndex[GameConstants::maxPlayers];
      string
        networkPlayerUUID[GameConstants::maxPlayers];


      int
        thisFactionIndex;
      int
        factionCount;
      int
        teams[GameConstants::maxPlayers];
      int
        startLocationIndex[GameConstants::maxPlayers];
      int
        mapFilter;

      int
        fallbackCpuMultiplier;
      bool
        defaultUnits;
      bool
        defaultResources;
      bool
        defaultVictoryConditions;

      bool
        fogOfWar;
      bool
        allowObservers;
      bool
        enableObserverModeAtEndGame;
      bool
        enableServerControlledAI;
      int
        networkFramePeriod;
      bool
        networkPauseGameForLaggedClients;
      PathFinderType
        pathFinderType;

      uint32
        flagTypes1;

      uint32
        mapCRC;
      uint32
        tilesetCRC;
      uint32
        techCRC;
      vector <
        pair <
        string,
      uint32 > >
        factionCRCList;

      int
        aiAcceptSwitchTeamPercentChance;
      int
        masterserver_admin;

      int
        masterserver_admin_factionIndex;

      bool
        networkAllowNativeLanguageTechtree;

      string
        gameUUID;

    public:

      static string
        playerDisconnectedText;

      GameSettings ()
      {
        defaultUnits = false;
        defaultResources = false;
        defaultVictoryConditions = false;
        mapFilter = 0;
        factionCount = 0;
        thisFactionIndex = 0;
        fogOfWar = true;
        allowObservers = false;
        enableObserverModeAtEndGame = false;
        enableServerControlledAI = false;
        networkFramePeriod = GameConstants::networkFramePeriod;
        networkPauseGameForLaggedClients = false;
        pathFinderType = pfBasic;

        static const string
          DEFAULT_LANG = "english";
        for (int i = 0; i < GameConstants::maxPlayers; ++i)
          {
            factionTypeNames[i] = "";
            networkPlayerNames[i] = "";
            networkPlayerPlatform[i] = "";
            networkPlayerStatuses[i] = npst_None;
            networkPlayerLanguages[i] = DEFAULT_LANG;
            factionControls[i] = ctClosed;
            resourceMultiplierIndex[i] = 1.0f;
            teams[i] = 0;
            startLocationIndex[i] = i;
            networkPlayerGameStatus[i] = 0;

            networkPlayerUUID[i] = "";
          }

        flagTypes1 = ft1_none;

        mapCRC = 0;
        tilesetCRC = 0;
        techCRC = 0;
        factionCRCList.
        clear ();
        aiAcceptSwitchTeamPercentChance = 30;
        masterserver_admin = -1;
        masterserver_admin_factionIndex = -1;
        fallbackCpuMultiplier = 1.0f;
        networkAllowNativeLanguageTechtree = true;
      }

      // default copy constructor will do fine, and will maintain itself ;)

      //get
      const
        string &
      getGameName () const
      {
        return
          gameName;
      }
      const
        string &
      getDescription () const
      {
        return
          description;
      }
      const
        string &
      getMap () const
      {
        return
          map;
      }
      const
        string &
      getTileset () const
      {
        return
          tileset;
      }
      const
        string &
      getTech () const
      {
        return
          tech;
      }
      const
        string &
      getScenario () const
      {
        return
          scenario;
      }
      const
        string &
      getScenarioDir () const
      {
        return
          scenarioDir;
      }
      const
        string &
      getFactionTypeName (int factionIndex) const
      {
        if (factionIndex == -1)
          {
            static string
              HEADLESS_FACTION = "headless-server";
            return
              HEADLESS_FACTION;
          }
        if (factionIndex < 0 || factionIndex >= GameConstants::maxPlayers)
          {
            char
              szBuf[8096] = "";
            snprintf (szBuf, 8096, "In [%s] Invalid factionIndex = %d\n",
                      __FUNCTION__, factionIndex);
            throw
            megaglest_runtime_error (szBuf);
          }
        return factionTypeNames[factionIndex];
      }
      string
      getNetworkPlayerName (int factionIndex) const
      {
        if (factionIndex < 0 || factionIndex >= GameConstants::maxPlayers)
          {
            char
              szBuf[8096] = "";
            snprintf (szBuf, 8096, "In [%s] Invalid factionIndex = %d\n",
                      __FUNCTION__, factionIndex);
            throw
            megaglest_runtime_error (szBuf);
          }

        string
          result = networkPlayerNames[factionIndex];
        if (networkPlayerStatuses[factionIndex] == npst_Disconnected)
          {
            result = playerDisconnectedText + result;
          }
        return result;
      }
      string
      getNetworkPlayerPlatform (int factionIndex) const
      {
        if (factionIndex < 0 || factionIndex >= GameConstants::maxPlayers)
          {
            char
              szBuf[8096] = "";
            snprintf (szBuf, 8096, "In [%s] Invalid factionIndex = %d\n",
                      __FUNCTION__, factionIndex);
            throw
            megaglest_runtime_error (szBuf);
          }

        string
          result = networkPlayerPlatform[factionIndex];
        return result;
      }

      const int
      getNetworkPlayerStatuses (int factionIndex) const
      {
        if (factionIndex < 0 || factionIndex >= GameConstants::maxPlayers)
          {
            char
              szBuf[8096] = "";
            snprintf (szBuf, 8096, "In [%s] Invalid factionIndex = %d\n",
                      __FUNCTION__, factionIndex);
            throw
            megaglest_runtime_error (szBuf);
          }

        return
          networkPlayerStatuses[factionIndex];
      }
      const string
      getNetworkPlayerLanguages (int factionIndex) const
      {
        if (factionIndex < 0 || factionIndex >= GameConstants::maxPlayers)
          {
            char
              szBuf[8096] = "";
            snprintf (szBuf, 8096, "In [%s] Invalid factionIndex = %d\n",
                      __FUNCTION__, factionIndex);
            throw
            megaglest_runtime_error (szBuf);
          }

        return
          networkPlayerLanguages[factionIndex];
      }

      const int
      getNetworkPlayerGameStatus (int factionIndex) const
      {
        if (factionIndex < 0 || factionIndex >= GameConstants::maxPlayers)
          {
            char
              szBuf[8096] = "";
            snprintf (szBuf, 8096, "In [%s] Invalid factionIndex = %d\n",
                      __FUNCTION__, factionIndex);
            throw
            megaglest_runtime_error (szBuf);
          }

        return
          networkPlayerGameStatus[factionIndex];
      }

      const
        vector <
        string >
      getUniqueNetworkPlayerLanguages () const
      {
        vector <
          string >
          languageList;
        for (int i = 0; i < GameConstants::maxPlayers; ++i)
          {
            if (networkPlayerLanguages[i] != "")
              {
                if (std::
                    find (languageList.begin (), languageList.end (),
                          networkPlayerLanguages[i]) == languageList.end ())
                  {
                    languageList.push_back (networkPlayerLanguages[i]);
                  }
              }
          }
        if (languageList.empty () == true)
          {
            languageList.push_back ("");
          }
        return languageList;
      }

      const string
      getNetworkPlayerNameByPlayerIndex (int playerIndex) const
      {
        string
          result = "";
        for (int i = 0; i < GameConstants::maxPlayers; ++i)
          {
            if (startLocationIndex[i] == playerIndex)
              {
                result = networkPlayerNames[i];
                break;
              }
          }
        return
          result;
      }
      const string
      getNetworkPlayerPlatformByPlayerIndex (int playerIndex) const
      {
        string
          result = "";
        for (int i = 0; i < GameConstants::maxPlayers; ++i)
          {
            if (startLocationIndex[i] == playerIndex)
              {
                result = networkPlayerPlatform[i];
                break;
              }
          }
        return
          result;
      }

      ControlType
      getFactionControl (int factionIndex) const
      {
        if (factionIndex < 0 || factionIndex >= GameConstants::maxPlayers)
          {
            char
              szBuf[8096] = "";
            snprintf (szBuf, 8096, "In [%s] Invalid factionIndex = %d\n",
                      __FUNCTION__, factionIndex);
            throw
            megaglest_runtime_error (szBuf);
          }

        return
          factionControls[factionIndex];
      }
      int
      getResourceMultiplierIndex (int factionIndex) const
      {
        if (factionIndex < 0 || factionIndex >= GameConstants::maxPlayers)
          {
            char
              szBuf[8096] = "";
            snprintf (szBuf, 8096, "In [%s] Invalid factionIndex = %d\n",
                      __FUNCTION__, factionIndex);
            throw
            megaglest_runtime_error (szBuf);
          }

        return
          resourceMultiplierIndex[factionIndex];
      }

      const
        string &
      getNetworkPlayerUUID (int factionIndex) const
      {
        if (factionIndex < 0 || factionIndex >= GameConstants::maxPlayers)
          {
            char
              szBuf[8096] = "";
            snprintf (szBuf, 8096, "In [%s] Invalid factionIndex = %d\n",
                      __FUNCTION__, factionIndex);
            throw
            megaglest_runtime_error (szBuf);
          }
        return
          networkPlayerUUID[factionIndex];
      }

      bool
      isNetworkGame () const
      {
        bool
          result = false;
        for (int idx = 0; idx < GameConstants::maxPlayers; ++idx)
          {
            if (factionControls[idx] == ctNetwork
                || factionControls[idx] == ctNetworkUnassigned
                || networkPlayerStatuses[idx] == npst_Disconnected)
              {
                result = true;
                break;
              }
          }
        return
          result;
      }
      int
      getThisFactionIndex () const
      {
        return
          thisFactionIndex;
      }
      int
      getFactionCount () const
      {
        return
          factionCount;
      }
      int
      getTeam (int factionIndex) const
      {
        if (factionIndex < 0 || factionIndex >= GameConstants::maxPlayers)
          {
            char
              szBuf[8096] = "";
            snprintf (szBuf, 8096, "In [%s] Invalid factionIndex = %d\n",
                      __FUNCTION__, factionIndex);
            throw
            megaglest_runtime_error (szBuf);
          }

        return
          teams[factionIndex];
      }

      int
      getStartLocationIndex (int factionIndex) const
      {
        if (factionIndex < 0 || factionIndex >= GameConstants::maxPlayers)
          {
            char
              szBuf[8096] = "";
            snprintf (szBuf, 8096, "In [%s] Invalid factionIndex = %d\n",
                      __FUNCTION__, factionIndex);
            throw
            megaglest_runtime_error (szBuf);
          }

        return
          startLocationIndex[factionIndex];
      }
      int
      getFactionIndexForStartLocation (int startIndex) const
      {
        if (startIndex < 0 || startIndex >= GameConstants::maxPlayers)
          {
            char
              szBuf[8096] = "";
            snprintf (szBuf, 8096, "In [%s] Invalid startIndex = %d\n",
                      __FUNCTION__, startIndex);
            throw
            megaglest_runtime_error (szBuf);
          }

        for (int i = 0; i < GameConstants::maxPlayers; ++i)
          {
            if (startLocationIndex[i] == startIndex)
              {
                return i;
              }
          }
        return -1;
      }

      int
      getMapFilter () const
      {
        return
          mapFilter;
      }

      bool
      getDefaultUnits () const
      {
        return
          defaultUnits;
      }
      bool
      getDefaultResources () const
      {
        return
          defaultResources;
      }
      bool
      getDefaultVictoryConditions () const
      {
        return
          defaultVictoryConditions;
      }

      bool
      getFogOfWar () const
      {
        return
          fogOfWar;
      }
      bool
      getAllowObservers () const
      {
        return
          allowObservers;
      }
      bool
      getEnableObserverModeAtEndGame () const
      {
        return
          enableObserverModeAtEndGame;
      }
      bool
      getEnableServerControlledAI () const
      {
        return
          enableServerControlledAI;
      }
      int
      getNetworkFramePeriod () const
      {
        return
          networkFramePeriod;
      }
      bool
      getNetworkPauseGameForLaggedClients () const
      {
        return
          networkPauseGameForLaggedClients;
      }
      PathFinderType
      getPathFinderType () const
      {
        return
          pathFinderType;
      }
      uint32
      getFlagTypes1 () const
      {
        return
          flagTypes1;
      }

      uint32
      getMapCRC () const
      {
        return
          mapCRC;
      }
      uint32
      getTilesetCRC () const
      {
        return
          tilesetCRC;
      }
      uint32
      getTechCRC () const
      {
        return
          techCRC;
      }
      vector <
        pair <
        string,
      uint32 > >
      getFactionCRCList () const
      {
        return
          factionCRCList;
      }

      const
        string &
      getGameUUID () const
      {
        return
          gameUUID;
      }

      //set
      void
      setGameName (const string & gameName)
      {
        this->gameName = gameName;
      }
      void
      setDescription (const string & description)
      {
        this->description = description;
      }
      void
      setMap (const string & map)
      {
        this->map = map;
      }
      void
      setTileset (const string & tileset)
      {
        this->tileset = tileset;
      }
      void
      setTech (const string & tech)
      {
        this->tech = tech;
      }
      void
      setScenario (const string & scenario)
      {
        this->scenario = scenario;
      }
      void
      setScenarioDir (const string & scenarioDir)
      {
        this->scenarioDir = scenarioDir;
      }

      void
      setFactionTypeName (int factionIndex, const string & factionTypeName)
      {
        if (factionIndex < 0 || factionIndex >= GameConstants::maxPlayers)
          {
            char
              szBuf[8096] = "";
            snprintf (szBuf, 8096, "In [%s] Invalid factionIndex = %d\n",
                      __FUNCTION__, factionIndex);
            throw
            megaglest_runtime_error (szBuf);
          }

        this->factionTypeNames[factionIndex] = factionTypeName;
      }
      void
      setNetworkPlayerName (int factionIndex, const string & playername)
      {
        if (factionIndex < 0 || factionIndex >= GameConstants::maxPlayers)
          {
            char
              szBuf[8096] = "";
            snprintf (szBuf, 8096, "In [%s] Invalid factionIndex = %d\n",
                      __FUNCTION__, factionIndex);
            throw
            megaglest_runtime_error (szBuf);
          }

        this->networkPlayerNames[factionIndex] = playername;
      }
      void
      setNetworkPlayerPlatform (int factionIndex, const string & platform)
      {
        if (factionIndex < 0 || factionIndex >= GameConstants::maxPlayers)
          {
            char
              szBuf[8096] = "";
            snprintf (szBuf, 8096, "In [%s] Invalid factionIndex = %d\n",
                      __FUNCTION__, factionIndex);
            throw
            megaglest_runtime_error (szBuf);
          }

        this->networkPlayerPlatform[factionIndex] = platform;
      }

      void
      setNetworkPlayerStatuses (int factionIndex, int status)
      {
        if (factionIndex < 0 || factionIndex >= GameConstants::maxPlayers)
          {
            char
              szBuf[8096] = "";
            snprintf (szBuf, 8096, "In [%s] Invalid factionIndex = %d\n",
                      __FUNCTION__, factionIndex);
            throw
            megaglest_runtime_error (szBuf);
          }

        this->networkPlayerStatuses[factionIndex] = status;
      }

      void
      setNetworkPlayerGameStatus (int factionIndex, int status)
      {
        if (factionIndex < 0 || factionIndex >= GameConstants::maxPlayers)
          {
            char
              szBuf[8096] = "";
            snprintf (szBuf, 8096, "In [%s] Invalid factionIndex = %d\n",
                      __FUNCTION__, factionIndex);
            throw
            megaglest_runtime_error (szBuf);
          }

        this->networkPlayerGameStatus[factionIndex] = status;
      }
      void
      setNetworkPlayerLanguages (int factionIndex, const string & language)
      {
        if (factionIndex < 0 || factionIndex >= GameConstants::maxPlayers)
          {
            char
              szBuf[8096] = "";
            snprintf (szBuf, 8096, "In [%s] Invalid factionIndex = %d\n",
                      __FUNCTION__, factionIndex);
            throw
            megaglest_runtime_error (szBuf);
          }

        this->networkPlayerLanguages[factionIndex] = language;
      }

      void
      setFactionControl (int factionIndex, ControlType controller)
      {
        if (factionIndex < 0 || factionIndex >= GameConstants::maxPlayers)
          {
            char
              szBuf[8096] = "";
            snprintf (szBuf, 8096, "In [%s] Invalid factionIndex = %d\n",
                      __FUNCTION__, factionIndex);
            throw
            megaglest_runtime_error (szBuf);
          }

        this->factionControls[factionIndex] = controller;
      }
      void
      setResourceMultiplierIndex (int factionIndex, int multiplierIndex)
      {
        if (factionIndex < 0 || factionIndex >= GameConstants::maxPlayers)
          {
            char
              szBuf[8096] = "";
            snprintf (szBuf, 8096, "In [%s] Invalid factionIndex = %d\n",
                      __FUNCTION__, factionIndex);
            throw
            megaglest_runtime_error (szBuf);
          }

        //if(multiplierIndex >= 45) {
        //      printf("gamesettings Line: %d multiplier index: %d factionIndex: %d\n",__LINE__,multiplierIndex,factionIndex);
        //}

        this->resourceMultiplierIndex[factionIndex] = multiplierIndex;
      }

      void
      setNetworkPlayerUUID (int factionIndex, const string & uuid)
      {
        if (factionIndex < 0 || factionIndex >= GameConstants::maxPlayers)
          {
            char
              szBuf[8096] = "";
            snprintf (szBuf, 8096, "In [%s] Invalid factionIndex = %d\n",
                      __FUNCTION__, factionIndex);
            throw
            megaglest_runtime_error (szBuf);
          }

        this->networkPlayerUUID[factionIndex] = uuid;
      }

      void
      setThisFactionIndex (int thisFactionIndex)
      {
        this->thisFactionIndex = thisFactionIndex;
      }
      void
      setFactionCount (int factionCount)
      {
        this->factionCount = factionCount;
      }
      void
      setTeam (int factionIndex, int team)
      {
        if (factionIndex < 0 || factionIndex >= GameConstants::maxPlayers)
          {
            char
              szBuf[8096] = "";
            snprintf (szBuf, 8096, "In [%s] Invalid factionIndex = %d\n",
                      __FUNCTION__, factionIndex);
            throw
            megaglest_runtime_error (szBuf);
          }

        this->teams[factionIndex] = team;
      }
      void
      setStartLocationIndex (int factionIndex, int startLocationIndex)
      {
        if (factionIndex < 0 || factionIndex >= GameConstants::maxPlayers)
          {
            char
              szBuf[8096] = "";
            snprintf (szBuf, 8096, "In [%s] Invalid factionIndex = %d\n",
                      __FUNCTION__, factionIndex);
            throw
            megaglest_runtime_error (szBuf);
          }

        this->startLocationIndex[factionIndex] = startLocationIndex;
      }
      void
      setMapFilter (int mapFilter)
      {
        this->mapFilter = mapFilter;
      }

      void
      setDefaultUnits (bool defaultUnits)
      {
        this->defaultUnits = defaultUnits;
      }
      void
      setDefaultResources (bool defaultResources)
      {
        this->defaultResources = defaultResources;
      }
      void
      setDefaultVictoryConditions (bool defaultVictoryConditions)
      {
        this->defaultVictoryConditions = defaultVictoryConditions;
      }

      void
      setFogOfWar (bool fogOfWar)
      {
        this->fogOfWar = fogOfWar;
      }
      void
      setAllowObservers (bool value)
      {
        this->allowObservers = value;
      }
      void
      setEnableObserverModeAtEndGame (bool value)
      {
        this->enableObserverModeAtEndGame = value;
      }
      void
      setEnableServerControlledAI (bool value)
      {
        this->enableServerControlledAI = value;
      }
      void
      setNetworkFramePeriod (int value)
      {
        this->networkFramePeriod = value;
      }
      void
      setNetworkPauseGameForLaggedClients (bool value)
      {
        this->networkPauseGameForLaggedClients = value;
      }
      void
      setPathFinderType (PathFinderType value)
      {
        this->pathFinderType = value;
      }

      void
      setFlagTypes1 (uint32 value)
      {
        this->flagTypes1 = value;
      }

      void
      setMapCRC (uint32 value)
      {
        mapCRC = value;
      }
      void
      setTilesetCRC (uint32 value)
      {
        tilesetCRC = value;
      }
      void
      setTechCRC (uint32 value)
      {
        techCRC = value;
      }

      void
      setFactionCRCList (const vector < pair < string, uint32 > >&value)
      {
        factionCRCList = value;
      }

      int
      getAiAcceptSwitchTeamPercentChance () const
      {
        return
          aiAcceptSwitchTeamPercentChance;
      }
      void
      setAiAcceptSwitchTeamPercentChance (int value)
      {
        aiAcceptSwitchTeamPercentChance = value;
      }

      int
      getFallbackCpuMultiplier () const
      {
        return
          fallbackCpuMultiplier;
      }
      void
      setFallbackCpuMultiplier (int value)
      {
        fallbackCpuMultiplier = value;
      }

      int
      getMasterserver_admin () const
      {
        return
          masterserver_admin;
      }
      void
      setMasterserver_admin (int value)
      {
        masterserver_admin = value;
      }

      int
      getMasterserver_admin_faction_index () const
      {
        return
          masterserver_admin_factionIndex;
      }
      void
      setMasterserver_admin_faction_index (int value)
      {
        masterserver_admin_factionIndex = value;
      }

      bool
      getNetworkAllowNativeLanguageTechtree () const
      {
        return
          networkAllowNativeLanguageTechtree;
      }
      void
      setNetworkAllowNativeLanguageTechtree (bool value)
      {
        networkAllowNativeLanguageTechtree = value;
      }

      void
      setGameUUID (const string & gameUUID)
      {
        this->gameUUID = gameUUID;
      }

      string
      toString () const
      {
        string
          result = "";

        result +=
          "Game ID = " +
          gameUUID +
          "\n";
        result +=
          "gameName = " +
          gameName +
          "\n";
        result +=
          "description = " +
          description +
          "\n";
        result +=
          "mapFilterIndex = " +
        intToStr (mapFilter) +
          "\n";
        result +=
          "map = " +
          map +
          "\n";
        result +=
          "tileset = " +
          tileset +
          "\n";
        result +=
          "tech = " +
          tech +
          "\n";
        result +=
          "scenario = " +
          scenario +
          "\n";
        result +=
          "scenarioDir = " +
          scenarioDir +
          "\n";

        for (int idx = 0; idx < GameConstants::maxPlayers; idx++)
          {
            result += "player index = " + intToStr (idx) + "\n";
            result +=
              "factionTypeName = " +
              factionTypeNames[idx] +
              "\n";
            result +=
              "networkPlayerName = " +
              networkPlayerNames[idx] +
              "\n";
            result +=
              "networkPlayerPlatform = " +
              networkPlayerPlatform[idx] +
              "\n";
            result +=
              "networkPlayerLanguage = " +
              networkPlayerLanguages[idx] +
              "\n";

            result +=
              "factionControl = " +
            intToStr (factionControls[idx]) +
              "\n";
            result +=
              "resourceMultiplierIndex = " +
            intToStr (resourceMultiplierIndex[idx]) +
              "\n";
            result +=
              "team = " +
            intToStr (teams[idx]) +
              "\n";
            result +=
              "startLocationIndex = " +
            intToStr (startLocationIndex[idx]) +
              "\n";
            result +=
              "networkPlayerUUID = " +
              networkPlayerUUID[idx] +
              "\n";
          }

        result +=
          "thisFactionIndex = " +
        intToStr (thisFactionIndex) +
          "\n";
        result += "factionCount = " + intToStr (factionCount) + "\n";
        result += "defaultUnits = " + intToStr (defaultUnits) + "\n";
        result += "defaultResources = " + intToStr (defaultResources) + "\n";
        result +=
          "defaultVictoryConditions = " +
          intToStr (defaultVictoryConditions) + "\n";
        result += "fogOfWar = " + intToStr (fogOfWar) + "\n";
        result += "allowObservers = " + intToStr (allowObservers) + "\n";
        result +=
          "enableObserverModeAtEndGame = " +
          intToStr (enableObserverModeAtEndGame) + "\n";
        result +=
          "enableServerControlledAI = " +
          intToStr (enableServerControlledAI) + "\n";
        result +=
          "networkFramePeriod = " + intToStr (networkFramePeriod) + "\n";
        result +=
          "networkPauseGameForLaggedClients = " +
          intToStr (networkPauseGameForLaggedClients) + "\n";
        result += "pathFinderType = " + intToStr (pathFinderType) + "\n";
        result += "flagTypes1 = " + uIntToStr (flagTypes1) + "\n";
        result += "mapCRC = " + uIntToStr (mapCRC) + "\n";
        result += "tilesetCRC = " + uIntToStr (tilesetCRC) + "\n";
        result += "techCRC = " + uIntToStr (techCRC) + "\n";

        for (unsigned int i = 0; i < factionCRCList.size (); ++i)
          {
            result +=
              "factionCRCList name [" + factionCRCList[i].first + "] CRC = " +
              uIntToStr (factionCRCList[i].second) + "\n";
          }

        result +=
          "aiAcceptSwitchTeamPercentChance = " +
          intToStr (aiAcceptSwitchTeamPercentChance) + "\n";
        result +=
          "masterserver_admin = " + intToStr (masterserver_admin) + "\n";
        result +=
          "masterserver_admin_factionIndex = " +
          intToStr (masterserver_admin_factionIndex) + "\n";

        result +=
          "networkAllowNativeLanguageTechtree = " +
          intToStr (networkAllowNativeLanguageTechtree) + "\n";
        return result;
      }

      void
      saveGame (XmlNode * rootNode) const
      {
        std::map <
          string,
          string >
          mapTagReplacements;
        XmlNode *
          gameSettingsNode = rootNode->addChild ("GameSettings");

        gameSettingsNode->
        addAttribute ("gameUUID", gameUUID, mapTagReplacements);

//              string gameName;
        gameSettingsNode->
        addAttribute ("gameName", gameName, mapTagReplacements);
//              string description;
        gameSettingsNode->
        addAttribute ("description", description, mapTagReplacements);
//              string map;
        gameSettingsNode->
        addAttribute ("map", map, mapTagReplacements);
//              string tileset;
        gameSettingsNode->
        addAttribute ("tileset", tileset, mapTagReplacements);
//              string tech;
        gameSettingsNode->
        addAttribute ("tech", tech, mapTagReplacements);
//              string scenario;
        gameSettingsNode->
        addAttribute ("scenario", scenario, mapTagReplacements);
//              string scenarioDir;
        gameSettingsNode->
        addAttribute ("scenarioDir", scenarioDir, mapTagReplacements);
//              string factionTypeNames[GameConstants::maxPlayers]; //faction names
        for (int idx = 0; idx < GameConstants::maxPlayers; idx++)
          {
            XmlNode *
              factionTypeNamesNode =
              gameSettingsNode->addChild ("factionTypeNames");
            factionTypeNamesNode->
            addAttribute ("name", factionTypeNames[idx], mapTagReplacements);
          }

//              string networkPlayerNames[GameConstants::maxPlayers];
        for (int idx = 0; idx < GameConstants::maxPlayers; idx++)
          {
            XmlNode *
              networkPlayerNamesNode =
              gameSettingsNode->addChild ("networkPlayerNames");
            networkPlayerNamesNode->addAttribute ("name",
                                                  networkPlayerNames[idx],
                                                  mapTagReplacements);
          }

        for (int idx = 0; idx < GameConstants::maxPlayers; idx++)
          {
            XmlNode *
              networkPlayerNamesNode =
              gameSettingsNode->addChild ("networkPlayerPlatform");
            networkPlayerNamesNode->addAttribute ("name",
                                                  networkPlayerPlatform[idx],
                                                  mapTagReplacements);
          }

//              int    networkPlayerStatuses[GameConstants::maxPlayers];
        for (int idx = 0; idx < GameConstants::maxPlayers; idx++)
          {
            XmlNode *
              networkPlayerStatusesNode =
              gameSettingsNode->addChild ("networkPlayerStatuses");
            networkPlayerStatusesNode->addAttribute ("status",
                                                     intToStr
                                                     (networkPlayerStatuses
                                                      [idx]),
                                                     mapTagReplacements);
          }

        //              int    networkPlayerStatuses[GameConstants::maxPlayers];
        for (int idx = 0; idx < GameConstants::maxPlayers; idx++)
          {
            XmlNode *
              networkPlayerStatusesNode =
              gameSettingsNode->addChild ("networkPlayerGameStatus");
            networkPlayerStatusesNode->addAttribute ("game_status",
                                                     intToStr
                                                     (networkPlayerGameStatus
                                                      [idx]),
                                                     mapTagReplacements);
          }

//              string networkPlayerLanguages[GameConstants::maxPlayers];
        for (int idx = 0; idx < GameConstants::maxPlayers; idx++)
          {
            XmlNode *
              networkPlayerLanguagesNode =
              gameSettingsNode->addChild ("networkPlayerLanguages");
            networkPlayerLanguagesNode->addAttribute ("name",
                                                      networkPlayerLanguages
                                                      [idx],
                                                      mapTagReplacements);
          }

//              ControlType factionControls[GameConstants::maxPlayers];
        for (int idx = 0; idx < GameConstants::maxPlayers; idx++)
          {
            XmlNode *
              factionControlsNode =
              gameSettingsNode->addChild ("factionControls");
            factionControlsNode->addAttribute ("control",
                                               intToStr (factionControls
                                                         [idx]),
                                               mapTagReplacements);
          }

//              int resourceMultiplierIndex[GameConstants::maxPlayers];
        for (int idx = 0; idx < GameConstants::maxPlayers; idx++)
          {
            XmlNode *
              resourceMultiplierIndexNode =
              gameSettingsNode->addChild ("resourceMultiplierIndex");
            resourceMultiplierIndexNode->addAttribute ("multiplier",
                                                       intToStr
                                                       (resourceMultiplierIndex
                                                        [idx]),
                                                       mapTagReplacements);
          }

//              int thisFactionIndex;
        gameSettingsNode->addAttribute ("thisFactionIndex",
                                        intToStr (thisFactionIndex),
                                        mapTagReplacements);
//              int factionCount;
        gameSettingsNode->addAttribute ("factionCount",
                                        intToStr (factionCount),
                                        mapTagReplacements);
//              int teams[GameConstants::maxPlayers];
        for (int idx = 0; idx < GameConstants::maxPlayers; idx++)
          {
            XmlNode *
              teamsNode = gameSettingsNode->addChild ("teams");
            teamsNode->addAttribute ("team", intToStr (teams[idx]),
                                     mapTagReplacements);
          }

//              int startLocationIndex[GameConstants::maxPlayers];
        for (int idx = 0; idx < GameConstants::maxPlayers; idx++)
          {
            XmlNode *
              startLocationIndexNode =
              gameSettingsNode->addChild ("startLocationIndex");
            startLocationIndexNode->addAttribute ("location",
                                                  intToStr (startLocationIndex
                                                            [idx]),
                                                  mapTagReplacements);
          }

        for (int idx = 0; idx < GameConstants::maxPlayers; idx++)
          {
            XmlNode *
              networkPlayerUUIDNode =
              gameSettingsNode->addChild ("networkPlayerUUID");
            networkPlayerUUIDNode->addAttribute ("value",
                                                 networkPlayerUUID[idx],
                                                 mapTagReplacements);
          }

//              int mapFilterIndex;
        gameSettingsNode->addAttribute ("mapFilterIndex",
                                        intToStr (mapFilter),
                                        mapTagReplacements);
//
//
//              bool defaultUnits;
        gameSettingsNode->addAttribute ("defaultUnits",
                                        intToStr (defaultUnits),
                                        mapTagReplacements);
//              bool defaultResources;
        gameSettingsNode->addAttribute ("defaultResources",
                                        intToStr (defaultResources),
                                        mapTagReplacements);
//              bool defaultVictoryConditions;
        gameSettingsNode->addAttribute ("defaultVictoryConditions",
                                        intToStr (defaultVictoryConditions),
                                        mapTagReplacements);
//              bool fogOfWar;
        gameSettingsNode->addAttribute ("fogOfWar", intToStr (fogOfWar),
                                        mapTagReplacements);
//              bool allowObservers;
        gameSettingsNode->addAttribute ("allowObservers",
                                        intToStr (allowObservers),
                                        mapTagReplacements);
//              bool enableObserverModeAtEndGame;
        gameSettingsNode->addAttribute ("enableObserverModeAtEndGame",
                                        intToStr
                                        (enableObserverModeAtEndGame),
                                        mapTagReplacements);
//              bool enableServerControlledAI;
        gameSettingsNode->addAttribute ("enableServerControlledAI",
                                        intToStr (enableServerControlledAI),
                                        mapTagReplacements);
//              int networkFramePeriod;
        gameSettingsNode->addAttribute ("networkFramePeriod",
                                        intToStr (networkFramePeriod),
                                        mapTagReplacements);
//              bool networkPauseGameForLaggedClients;
        gameSettingsNode->addAttribute ("networkPauseGameForLaggedClients",
                                        intToStr
                                        (networkPauseGameForLaggedClients),
                                        mapTagReplacements);
//              PathFinderType pathFinderType;
        gameSettingsNode->addAttribute ("pathFinderType",
                                        intToStr (pathFinderType),
                                        mapTagReplacements);
//              uint32 flagTypes1;
        gameSettingsNode->addAttribute ("flagTypes1", uIntToStr (flagTypes1),
                                        mapTagReplacements);
//          int32 mapCRC;
        gameSettingsNode->addAttribute ("mapCRC", uIntToStr (mapCRC),
                                        mapTagReplacements);
//          int32 tilesetCRC;
        gameSettingsNode->addAttribute ("tilesetCRC", uIntToStr (tilesetCRC),
                                        mapTagReplacements);
//          int32 techCRC;
        gameSettingsNode->addAttribute ("techCRC", uIntToStr (techCRC),
                                        mapTagReplacements);
//          vector<pair<string,int32> > factionCRCList;
        for (unsigned int i = 0; i < factionCRCList.size (); ++i)
          {
            const
              pair <
              string,
            uint32 > &
              item = factionCRCList[i];

            XmlNode *
              factionCRCListNode =
              gameSettingsNode->addChild ("factionCRCList");
            factionCRCListNode->addAttribute ("key", item.first,
                                              mapTagReplacements);
            factionCRCListNode->addAttribute ("value",
                                              uIntToStr (item.second),
                                              mapTagReplacements);
          }
//          int aiAcceptSwitchTeamPercentChance;
        gameSettingsNode->addAttribute ("aiAcceptSwitchTeamPercentChance",
                                        intToStr
                                        (aiAcceptSwitchTeamPercentChance),
                                        mapTagReplacements);
//          int masterserver_admin;
        gameSettingsNode->addAttribute ("masterserver_admin",
                                        intToStr (masterserver_admin),
                                        mapTagReplacements);

        gameSettingsNode->addAttribute ("masterserver_admin_factionIndex",
                                        intToStr
                                        (masterserver_admin_factionIndex),
                                        mapTagReplacements);

        gameSettingsNode->addAttribute ("networkAllowNativeLanguageTechtree",
                                        intToStr
                                        (networkAllowNativeLanguageTechtree),
                                        mapTagReplacements);
      }

      void
      loadGame (const XmlNode * rootNode)
      {
        const XmlNode *
          gameSettingsNode = rootNode->getChild ("GameSettings");

        if (gameSettingsNode->hasAttribute ("gameUUID") == true)
          {
            gameUUID =
              gameSettingsNode->getAttribute ("gameUUID")->getValue ();
          }

//              string gameName;
        if (gameSettingsNode->hasAttribute ("gameName") == true)
          {
            gameName =
              gameSettingsNode->getAttribute ("gameName")->getValue ();
          }
        else
          {
            gameName = "oldSavegame";
          }
//              string description;
        description =
          gameSettingsNode->getAttribute ("description")->getValue ();
//              string map;
        map = gameSettingsNode->getAttribute ("map")->getValue ();
//              string tileset;
        tileset = gameSettingsNode->getAttribute ("tileset")->getValue ();
//              string tech;
        tech = gameSettingsNode->getAttribute ("tech")->getValue ();
//              string scenario;
        scenario = gameSettingsNode->getAttribute ("scenario")->getValue ();
//              string scenarioDir;
        scenarioDir =
          gameSettingsNode->getAttribute ("scenarioDir")->getValue ();
        if (fileExists (scenarioDir) == false)
          {
            scenarioDir = Config::findValidLocalFileFromPath (scenarioDir);
          }

//              string factionTypeNames[GameConstants::maxPlayers]; //faction names
        for (int idx = 0; idx < GameConstants::maxPlayers; idx++)
          {
            const XmlNode *
              factionTypeNamesNode =
              gameSettingsNode->getChild ("factionTypeNames", idx);
            factionTypeNames[idx] =
              factionTypeNamesNode->getAttribute ("name")->getValue ();
          }

//              string networkPlayerNames[GameConstants::maxPlayers];
        for (int idx = 0; idx < GameConstants::maxPlayers; idx++)
          {
            const XmlNode *
              networkPlayerNamesNode =
              gameSettingsNode->getChild ("networkPlayerNames", idx);
            networkPlayerNames[idx] =
              networkPlayerNamesNode->getAttribute ("name")->getValue ();
          }

//              int    networkPlayerStatuses[GameConstants::maxPlayers];
        for (int idx = 0; idx < GameConstants::maxPlayers; idx++)
          {
            const XmlNode *
              networkPlayerStatusesNode =
              gameSettingsNode->getChild ("networkPlayerStatuses", idx);
            networkPlayerStatuses[idx] =
              networkPlayerStatusesNode->getAttribute ("status")->
              getIntValue ();
          }

        //              int    networkPlayerStatuses[GameConstants::maxPlayers];
        for (int idx = 0; idx < GameConstants::maxPlayers; idx++)
          {
            if (gameSettingsNode->
                hasChildAtIndex ("networkPlayerGameStatus", idx) == true)
              {
                const XmlNode *
                  networkPlayerGameStatusNode =
                  gameSettingsNode->getChild ("networkPlayerGameStatus", idx);
                networkPlayerGameStatus[idx] =
                  networkPlayerGameStatusNode->getAttribute ("game_status")->
                  getIntValue ();
              }
          }

//              string networkPlayerLanguages[GameConstants::maxPlayers];
        for (int idx = 0; idx < GameConstants::maxPlayers; idx++)
          {
            const XmlNode *
              networkPlayerLanguagesNode =
              gameSettingsNode->getChild ("networkPlayerLanguages", idx);
            networkPlayerLanguages[idx] =
              networkPlayerLanguagesNode->getAttribute ("name")->getValue ();
          }

//              ControlType factionControls[GameConstants::maxPlayers];
        for (int idx = 0; idx < GameConstants::maxPlayers; idx++)
          {
            const XmlNode *
              factionControlsNode =
              gameSettingsNode->getChild ("factionControls", idx);
            factionControls[idx] =
              static_cast < ControlType >
              (factionControlsNode->getAttribute ("control")->getIntValue ());
          }

//              int resourceMultiplierIndex[GameConstants::maxPlayers];
        for (int idx = 0; idx < GameConstants::maxPlayers; idx++)
          {
            const XmlNode *
              resourceMultiplierIndexNode =
              gameSettingsNode->getChild ("resourceMultiplierIndex", idx);
            resourceMultiplierIndex[idx] =
              resourceMultiplierIndexNode->getAttribute ("multiplier")->
              getIntValue ();
          }

        for (int idx = 0; idx < GameConstants::maxPlayers; idx++)
          {
            if (gameSettingsNode->
                hasChildAtIndex ("networkPlayerUUID", idx) == true)
              {
                const XmlNode *
                  networkPlayerUUIDNode =
                  gameSettingsNode->getChild ("networkPlayerUUID", idx);
                networkPlayerUUID[idx] =
                  networkPlayerUUIDNode->getAttribute ("value")->getValue ();
              }
          }

//              int thisFactionIndex;
        thisFactionIndex =
          gameSettingsNode->getAttribute ("thisFactionIndex")->getIntValue ();
//              int factionCount;
        factionCount =
          gameSettingsNode->getAttribute ("factionCount")->getIntValue ();
//              int teams[GameConstants::maxPlayers];
        for (int idx = 0; idx < GameConstants::maxPlayers; idx++)
          {
            const XmlNode *
              teamsNode = gameSettingsNode->getChild ("teams", idx);
            teams[idx] = teamsNode->getAttribute ("team")->getIntValue ();
          }

//              int startLocationIndex[GameConstants::maxPlayers];
        for (int idx = 0; idx < GameConstants::maxPlayers; idx++)
          {
            const XmlNode *
              startLocationIndexNode =
              gameSettingsNode->getChild ("startLocationIndex", idx);
            startLocationIndex[idx] =
              startLocationIndexNode->getAttribute ("location")->
              getIntValue ();
          }

//              int mapFilterIndex;
        mapFilter =
          gameSettingsNode->getAttribute ("mapFilterIndex")->getIntValue ();
//
//
//              bool defaultUnits;
        defaultUnits =
          gameSettingsNode->getAttribute ("defaultUnits")->getIntValue () !=
          0;
//              bool defaultResources;
        defaultResources =
          gameSettingsNode->getAttribute ("defaultResources")->
          getIntValue () != 0;
//              bool defaultVictoryConditions;
        defaultVictoryConditions =
          gameSettingsNode->getAttribute ("defaultVictoryConditions")->
          getIntValue () != 0;
//              bool fogOfWar;
        fogOfWar =
          gameSettingsNode->getAttribute ("fogOfWar")->getIntValue () != 0;
//              bool allowObservers;
        allowObservers =
          gameSettingsNode->getAttribute ("allowObservers")->getIntValue () !=
          0;
//              bool enableObserverModeAtEndGame;
        enableObserverModeAtEndGame =
          gameSettingsNode->getAttribute ("enableObserverModeAtEndGame")->
          getIntValue () != 0;
//              bool enableServerControlledAI;
        enableServerControlledAI =
          gameSettingsNode->getAttribute ("enableServerControlledAI")->
          getIntValue () != 0;
//              int networkFramePeriod;
        networkFramePeriod =
          gameSettingsNode->getAttribute ("networkFramePeriod")->
          getIntValue ();
//              bool networkPauseGameForLaggedClients;
        networkPauseGameForLaggedClients =
          gameSettingsNode->
          getAttribute ("networkPauseGameForLaggedClients")->getIntValue () !=
          0;
//              PathFinderType pathFinderType;
        pathFinderType =
          static_cast < PathFinderType >
          (gameSettingsNode->getAttribute ("pathFinderType")->getIntValue ());
//              uint32 flagTypes1;
        flagTypes1 =
          gameSettingsNode->getAttribute ("flagTypes1")->getUIntValue ();
//          int32 mapCRC;
        mapCRC = gameSettingsNode->getAttribute ("mapCRC")->getUIntValue ();
//          int32 tilesetCRC;
        tilesetCRC =
          gameSettingsNode->getAttribute ("tilesetCRC")->getUIntValue ();
//          int32 techCRC;
        techCRC = gameSettingsNode->getAttribute ("techCRC")->getUIntValue ();
//          vector<pair<string,int32> > factionCRCList;
//              for(unsigned int i = 0; i < factionCRCList.size(); ++i) {
//                      const pair<string,int32> &item = factionCRCList[i];
//
//                      XmlNode *factionCRCListNode = gameSettingsNode->addChild("factionCRCList");
//                      factionCRCListNode->addAttribute("key",item.first, mapTagReplacements);
//                      factionCRCListNode->addAttribute("value",intToStr(item.second), mapTagReplacements);
//              }
//          int aiAcceptSwitchTeamPercentChance;
        aiAcceptSwitchTeamPercentChance =
          gameSettingsNode->getAttribute ("aiAcceptSwitchTeamPercentChance")->
          getIntValue ();
//          int masterserver_admin;
        masterserver_admin =
          gameSettingsNode->getAttribute ("masterserver_admin")->
          getIntValue ();

        if (gameSettingsNode->
            hasAttribute ("masterserver_admin_factionIndex") == true)
          {
            masterserver_admin_factionIndex =
              gameSettingsNode->
              getAttribute ("masterserver_admin_factionIndex")->
              getIntValue ();
          }

        if (gameSettingsNode->
            hasAttribute ("networkAllowNativeLanguageTechtree") == true)
          {
            networkAllowNativeLanguageTechtree =
              gameSettingsNode->
              getAttribute ("networkAllowNativeLanguageTechtree")->
              getIntValue () != 0;
          }
      }

    };

  }
}                               //end namespace

#endif
