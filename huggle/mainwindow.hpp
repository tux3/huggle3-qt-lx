//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QInputDialog>
#include <QLabel>
#include <QDesktopServices>
#include <QMutex>
#include <QThread>
#include <QSplitter>
#include <QDockWidget>
#include "configuration.hpp"
#include "core.hpp"
#include "aboutform.hpp"
#include "preferences.hpp"
#include "hugglelog.hpp"
#include "hugglequeue.hpp"
#include "huggletool.hpp"
#include "huggleweb.hpp"
#include "wikipage.hpp"
#include "editquery.hpp"
#include "processlist.hpp"
#include "wikiuser.hpp"
#include "ignorelist.hpp"
#include "speedyform.hpp"
#include "exception.hpp"
#include "hooks.hpp"
#include "history.hpp"
#include "blockuser.hpp"
#include "hugglefeedproviderwiki.hpp"
#include "hugglefeedproviderirc.hpp"
#include "userinfoform.hpp"
#include "vandalnw.hpp"
#include "revertquery.hpp"
#include "whitelistform.hpp"
#include "collectable.hpp"
#include "gc.hpp"
#include "reportuser.hpp"
#include "waitingform.hpp"
#include "wlquery.hpp"
#include "sessionform.hpp"
#include "historyform.hpp"
#include "scorewordsdbform.hpp"
#include "deleteform.hpp"
#include "protectpage.hpp"
#include "uaareport.hpp"
#include "localization.hpp"
#include "syslog.hpp"
#include "huggleparser.hpp"

namespace Ui
{
    class MainWindow;
}

namespace Huggle
{
    class HuggleLog;
    class History;
    class HistoryForm;
    class UserinfoForm;
    class HuggleQueue;
    class HuggleTool;
    class AboutForm;
    class HuggleWeb;
    class SpeedyForm;
    class WikiEdit;
    class RevertQuery;
    class WlQuery;
    class WikiPage;
    class EditQuery;
    class ProcessList;
    class WhitelistForm;
    class Message;
    class Preferences;
    class SessionForm;
    class IgnoreList;
    class WaitingForm;
    class VandalNw;
    class Syslog;
    class WikiUser;
    class ReportUser;
    class DeleteForm;
    class BlockUser;
    class ProtectPage;
    class UAAReport;
    class HuggleParser;
    class ScoreWordsDbForm;

    /*!
     * \brief The ShutdownOp enum contains a various parts of shutdown so that we can keep the track of what is going on
     */
    enum ShutdownOp
    {
        //! Huggle is not shutting down
        ShutdownOpRunning,
        //! Huggle is downloading a whitelist in order to update it
        ShutdownOpRetrievingWhitelist,
        //! Huggle is updating the whitelist
        ShutdownOpUpdatingWhitelist,
        //! Huggle is updating a config of user
        ShutdownOpUpdatingConf
    };

    /*!
     * \brief The PendingWarning class represent the warning that was requested but might not be delievered
     *
     * We can be using this to track all warnings that are still waiting for delivery and these which have failed
     * because the user talk page was changed meanwhile. In this case we need to parse it again and re-issue
     * the warning based on latest version of talk page. The current process is, that every warning that is sent
     * is stored in a list of these pending warnings and periodically checked using timer. If it's finished it's
     * removed, if it's not then it is checked and something is done with it.
     */
    class PendingWarning
    {
        public:
            //! Unique garbage collector id used to lock the edit related to this warning
            static int GCID;

            PendingWarning(Message *message, QString warning, WikiEdit *edit);
            ~PendingWarning();
            //! The message of this warning
            Message *Warning;
            //! The edit because of which this warning was sent
            WikiEdit *RelatedEdit;
            //! Template used in this warning so that we can use the same template for new attempt if any is needed
            QString Template;
            int gcid;
            ApiQuery *Query;
    };

    //! Primary huggle window
    class MainWindow : public QMainWindow
    {
            Q_OBJECT

        public:
            explicit MainWindow(QWidget *parent = 0);
            ~MainWindow();
            void DisplayReportUserWindow(WikiUser *User = NULL);
            void ProcessEdit(WikiEdit *e, bool IgnoreHistory = false, bool KeepHistory = false, bool KeepUser = false);
            RevertQuery *Revert(QString summary = "", bool nd = false, bool next = true);
            //! Warn a current user
            bool Warn(QString WarningType, RevertQuery *dependency);
            //! Warn a specific user which may be unrelated to current edit
            bool WarnUser(QString WarningType, RevertQuery *dependency, WikiEdit *Edit);
            QString GetSummaryKey(QString item);
            QString GetSummaryText(QString text);
            //! Send a template to user no matter if they can be messaged or not
            void ForceWarn(int level);
            void Exit();
            void ReconnectIRC();
            //! Returns true if current page can be edited
            bool BrowserPageIsEditable();
            /*!
             * \brief CheckEditableBrowserPage will check if current page is editable and if it's not it display a message box
             * \return true on success or false in case it's not
             */
            bool CheckEditableBrowserPage();
            void SuspiciousEdit();
            void PatrolThis(WikiEdit *e = NULL);
            void Localize();
            //! Checks all warnings that weren't sent and try to send them

            //! This is used on talk pages of users which changed while we tried to send them a warning
            void ResendWarning();
            void _BlockUser();
            void DisplayNext(Query *q = NULL);
            void DeletePage();
            void DisplayTalk();
            //! List of edits that are being saved
            QList<WikiEdit*> PendingEdits;
            //! Pointer to syslog
            HuggleLog *SystemLog;
            //! Pointer to queue
            HuggleQueue *Queue1;
            QList<ApiQuery*> PatroledEdits;
            //! Pointer to browser
            HuggleWeb *Browser;
            HistoryForm *wHistory;
            UserinfoForm *wUserInfo;
            //! Pointer to toolbar
            HuggleTool *tb;
            //! Pointer to options
            Preferences *preferencesForm;
            //! Pointer to ignore list (see ignorelist.h)
            IgnoreList *Ignore;
            //! Pointer to about dialog (see aboutform.h)
            AboutForm *aboutForm;
            //! Pointer to current edit, if it's NULL there is no edit being displayed
            WikiEdit *CurrentEdit;
            SpeedyForm* fRemove;
            //! Pointer to processes
            ProcessList *Queries;
            //! Pointer to history
            History * _History;
            //! Pointer to menu of revert warn button
            QMenu *RevertWarn;
            //! Pointer to vandal network
            VandalNw * VandalDock;
            SessionForm *fSessionData;
            //! Pointer to query that is used to store user config on exit of huggle
            EditQuery *eq;
            //! This query is used to refresh white list
            WLQuery *wq;
            //! Warning menu
            QMenu *WarnMenu;
            //! Revert menu
            QMenu *RevertSummaries;
            ScoreWordsDbForm *fScoreWord;
            Ui::MainWindow *ui;
            bool ShuttingDown;
            //! If system is shutting down this is displaying which part of shutdown is currently being executed
            ShutdownOp Shutdown;
            ReportUser *fReportForm;
            //! Pointer to a form to block user
            BlockUser *fBlockForm;
            //! Pointer to a form to delete a page
            DeleteForm *fDeleteForm;
            //! Pointer to a form to protect a page
            ProtectPage *fProtectForm;
            //! Pointer to UAA dialog
            UAAReport *fUaaReportForm;
            WhitelistForm *fWhitelist;
            int LastTPRevID;
            //! This is a query for rollback of current edit which we need to keep in case
            //! that user wants to display their own revert instead of next page
            Query *qNext;
            //! Timer that is used to check if there are new messages on talk page
            QTimer *tCheck;
            //! Query that is used to check if talk page contains new messages
            ApiQuery *qTalkPage;

        private slots:
            void on_actionExit_triggered();
            void on_actionPreferences_triggered();
            void on_actionContents_triggered();
            void on_actionAbout_triggered();
            void OnTimerTick1();
            void OnTimerTick0();
            void on_actionNext_triggered();
            void on_actionNext_2_triggered();
            void on_actionWarn_triggered();
            void on_actionRevert_currently_displayed_edit_triggered();
            void on_actionWarn_the_user_triggered();
            void on_actionRevert_currently_displayed_edit_and_warn_the_user_triggered();
            void on_actionRevert_and_warn_triggered();
            void on_actionRevert_triggered();
            void on_actionShow_ignore_list_of_current_wiki_triggered();
            void on_actionForward_triggered();
            void on_actionBack_triggered();
            void CustomRevert();
            void CustomRevertWarn();
            void CustomWarn();
            void on_actionWelcome_user_triggered();
            void on_actionOpen_in_a_browser_triggered();
            void on_actionIncrease_badness_score_by_20_triggered();
            void on_actionDecrease_badness_score_by_20_triggered();
            void on_actionGood_edit_triggered();
            void on_actionTalk_page_triggered();
            void on_actionFlag_as_a_good_edit_triggered();
            void on_actionDisplay_this_page_in_browser_triggered();
            void on_actionEdit_page_in_browser_triggered();
            void on_actionDisplay_history_in_browser_triggered();
            void on_actionStop_feed_triggered();
            void on_actionRemove_old_edits_triggered();
            void on_actionClear_talk_page_of_user_triggered();
            void on_actionList_all_QGC_items_triggered();
            void on_actionRevert_currently_displayed_edit_warn_user_and_stay_on_page_triggered();
            void on_actionRevert_currently_displayed_edit_and_stay_on_page_triggered();
            void on_actionWelcome_user_2_triggered();
            void on_actionReport_user_triggered();
            void on_actionReport_user_2_triggered();
            void on_actionWarning_1_triggered();
            void on_actionWarning_2_triggered();
            void on_actionWarning_3_triggered();
            void on_actionWarning_4_triggered();
            void on_actionEdit_user_talk_triggered();
            void on_actionReconnect_IRC_triggered();
            void on_actionRequest_speedy_deletion_triggered();
            void on_actionDelete_triggered();
            void on_actionBlock_user_triggered();
            void on_actionIRC_triggered();
            void on_actionWiki_triggered();
            void on_actionProtect_triggered();
            void on_actionShow_talk_triggered();
            void on_actionEdit_info_triggered();
            void on_actionFlag_as_suspicious_edit_triggered();
            void on_actionDisconnect_triggered();
            void on_actionReport_username_triggered();
            void on_actionShow_list_of_score_words_triggered();
            void on_actionRevert_AGF_triggered();
            void on_actionDisplay_a_session_data_triggered();
            void on_actionDisplay_whitelist_triggered();
            void on_actionResort_queue_triggered();
            void on_actionRestore_this_revision_triggered();
            void on_actionClear_triggered();
            void on_actionDelete_page_triggered();
            void on_actionBlock_user_2_triggered();
            void on_actionDisplay_talk_triggered();
            void on_actionIncrease_badness_triggered();
            void on_actionDecrease_badness_triggered();
            void TimerCheckTPOnTick();
            void on_actionSimulate_message_triggered();
            void on_actionHtml_dump_triggered();
            void on_actionEnforce_sysop_rights_triggered();
            void on_actionFeedback_triggered();

        private:
            //! Check if huggle is shutting down or not, in case it is, message box is shown as well
            //! this function should be called before every action user can trigger
            bool CheckExit();
            void DisplayWelcomeMessage();
            void FinishRestore();
            //! Check if we can revert this edit
            bool PreflightCheck(WikiEdit *_e);
            //! Welcome user
            void Welcome();
            //! Recreate interface, should be called everytime you do anything with main form
            void Render();
            //! Request a page deletion csd or afd and so on
            void RequestPD();
            void closeEvent(QCloseEvent *event);
            void FinishPatrols();
            void DecreaseBS();
            void IncreaseBS();
            QTimer *timer1;
            QString RestoreEdit_RevertReason;
            // Whitelist
            QTimer *wlt;
            //! Status bar
            QLabel *Status;
            bool EditablePage;
            QList <PendingWarning*> Warnings;
            WaitingForm *fWaiting;
            //! List of all edits that are kept in history, so that we can track them and delete them
            QList <WikiEdit*> Historical;
            ApiQuery *RestoreQuery;
            WikiEdit *RestoreEdit;
            //! This is a page that is going to be displayed if users request their latest action to be
            //! reviewed when it's done (for example when they rollback an edit and they want to
            //! display it, instead of next one)
            WikiPage *OnNext_EvPage;
    };
}
#endif // MAINWINDOW_H
