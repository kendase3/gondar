#ifndef CHROMEOVER_LOGIN_PAGE_H
#define CHROMEOVER_LOGIN_PAGE_H

#include <vector>

#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>

#include "gondarsite.h"
#include "meepo.h"
#include "wizard_page.h"

class ChromeoverLoginPage : public gondar::WizardPage {
  Q_OBJECT

 public:
  ChromeoverLoginPage(QWidget* parent = 0);
  int nextId() const override;
  std::vector<GondarSite> siteList;

 protected:
  bool validatePage() override;

 private:
  void handleMeepoFinished();

  gondar::Meepo meepo_;

  QGridLayout layout;
  QLineEdit usernameLineEdit;
  QLabel usernameLineEditLabel;
  QLineEdit passwordLineEdit;
  QLabel passwordLineEditLabel;
  QLabel meanWordsLabel;
  bool finished;
  bool started;
};

#endif
